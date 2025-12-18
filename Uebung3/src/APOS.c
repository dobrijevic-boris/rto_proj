#include "APOS.h"

#include "stm32f0xx.h"
#include "StdDef.h"
#include "BSP/Debug.h"
#include "BSP/systick.h"
#include <string.h>
#define APOS_STACK_GUARD "STACKEND"
#define APOS_STACK_GUARD_SIZE (uint8_t)8

// --- global variables ---
APOS_TCB_STRUCT TCB_Tasks[APOS_TASK_NR];
APOS_TCB_STRUCT* pHead = NULL_PTR;           // pointer to first task in ready queue, NULL initialized
uint8_t systemInit = 0;

// --- static variables ---
static APOS_TCB_STRUCT* pCurrentTask = NULL_PTR;     // pointer to currently running task
static volatile uint32_t criticalSectionCnt = 0;
static volatile BOOL schedulerPending = FALSE;
static inline void APOS_AssertStackGuard(const void *pStack);
// function prototypes
void APOS_RemoveFromReadyQueue(APOS_TCB_STRUCT* pTask);

// === function definitions ===
static void TCB_Init(void) {
    for(uint32_t i=0; i<APOS_TASK_NR;i++) {
        TCB_Tasks[i].pRoutine   = 0;
        TCB_Tasks[i].Priority   = 0;
        TCB_Tasks[i].pStack     = 0;
        TCB_Tasks[i].pTaskName  = 0;
        TCB_Tasks[i].StackSize  = 0;
        TCB_Tasks[i].state      = APOS_TASK_SUSPENDED;
        TCB_Tasks[i].TimeLeft   = 0;
        TCB_Tasks[i].TimeSlice  = 0;
        TCB_Tasks[i].delay      = 0;
        TCB_Tasks[i].pNextRdy   = NULL_PTR;
    }
}

void APOS_Init (void) {
    NVIC_SetPriority(PendSV_IRQn, 3); 
    TCB_Init();
}

void APOS_Start (void) {

    if (pHead == 0) {
        while(1); // Fatal error: No tasks to run.
    }
    
    systemInit = 1;
    
    pCurrentTask = pHead;
    APOS_RemoveFromReadyQueue(pCurrentTask); // dequeue from ready queue
    pCurrentTask->state = APOS_TASK_RUNNING; // set task as running
    
    pCurrentTask->pStack = (uint32_t*)pCurrentTask->pStack + 16;
    uint32_t pc = *((uint32_t*)pCurrentTask->pStack-2);
    // switch to currentTask
    __set_PSP((uint32_t)pCurrentTask->pStack);  // PSP zeigt ans Ende des Arrays
    APOS_set_ctrl_pc(pc);
}

void APOS_INSERT_QUEUE(APOS_TCB_STRUCT* pTask) {

    if (pTask == NULL_PTR) return;

    pTask->pNextRdy = NULL_PTR;

    APOS_TCB_STRUCT* pCurrent = pHead;
    APOS_TCB_STRUCT* pPrev = NULL_PTR;

    // Traverse list to find insertion point. Higher number = higher priority.
    // For same-priority tasks, new task goes after existing ones.
    while(pCurrent != NULL_PTR && pTask->Priority <= pCurrent->Priority) {
        pPrev = pCurrent;
        pCurrent = pCurrent->pNextRdy;
    }
    
    pTask->pNextRdy = pCurrent;
    if (pPrev == NULL_PTR) {
        // Insert at the head
        pTask->pNextRdy = pHead;
        pHead = pTask;
    } 
    else {
        // Insert in middle or at end
        pPrev->pNextRdy = pTask;
        pTask->pNextRdy = pCurrent;
    }
}

void APOS_TASK_Create ( 
    APOS_TCB_STRUCT* pTask, // TaskControlBlock
    const char* pTaskName,  // Task Name nur fuer Debug-Zwecke
    uint32_t Priority,      // Prioritaet des Tasks
    void (*pRoutine)(void), // Startadresse Task (ROM)
    void * pStack,          // Startadresse Stack des Tasks (RAM)
    uint32_t StackSize,     // Groesse des Stacks
    uint32_t TimeSlice,     // Time Slice fuer Round Robin Scheduling  
    APOS_TASK_STATE state,  // task state (Ready, running, ...
    uint32_t delay,         // time delay in ticks (1ms)
    uint32_t TimeLeft)      // time left running         
{
    pTask->pTaskName = pTaskName;
    pTask->Priority  = Priority;
    pTask->pRoutine  = pRoutine;
    pTask->StackSize = StackSize;
    pTask->pStackEnd = pStack;
    pTask->TimeSlice = TimeSlice;
    pTask->state     = state;
    pTask->delay     = delay;
    pTask->TimeLeft  = TimeLeft > 0 ? TimeLeft : TimeSlice;
    pTask->pNextRdy  = NULL_PTR;

    // place task in ready queue if ready
    if (pTask->state == APOS_TASK_READY) { 
        //APOS_EnterCriticalRegion();
        APOS_INSERT_QUEUE(pTask);
        //APOS_ExitCriticalRegion();
    }
    
    // Init stack
    uint32_t* sp = (uint32_t*)pStack;
    
    memcpy(sp, APOS_STACK_GUARD, 8);
    sp += APOS_TASK_STACK_SZ;
    *(--sp) = 0x01000000;
    *(--sp) = (uint32_t)pRoutine;
    *(--sp) = 0xFFFFFFFD; // lr (value doesnt mather)
    *(--sp) = 0xCCCCCCCC; // r12
    *(--sp) = 0x33333333; // r3
    *(--sp) = 0x22222222; // r2
    *(--sp) = 0x11111111; // r1
    *(--sp) = 0x00000000; // r0
   
    *(--sp) = 0xBBBBBBBB; // R11
    *(--sp) = 0xAAAAAAAA; // R10
    *(--sp) = 0x99999999; // R9
    *(--sp) = 0x88888888; // R8
    *(--sp) = 0x77777777; // R7
    *(--sp) = 0x66666666; // R6
    *(--sp) = 0x55555555; // R5
    *(--sp) = 0x44444444; // R4 
    pTask->pStack = sp;
}

void APOS_RemoveFromReadyQueue(APOS_TCB_STRUCT* pTask) {
    
    if(pTask == pHead){
        pHead = pHead->pNextRdy;
        pTask->pNextRdy = NULL_PTR;
        return;
    }

    APOS_TCB_STRUCT* pCurrent = pHead;
    APOS_TCB_STRUCT* pPrev = NULL_PTR;
    while (pCurrent != pTask && pCurrent != NULL_PTR) {
        pPrev = pCurrent;
        pCurrent = pCurrent->pNextRdy;
    }
    
    if (pCurrent != NULL_PTR) {
        pPrev->pNextRdy = pCurrent->pNextRdy;
        pCurrent->pNextRdy = NULL_PTR;
    }
    else{
        // safety catch
        while(1){}
    }
    
}

uint32_t* APOS_Select_next_task(uint32_t *sp) {
    
    APOS_AssertStackGuard(pCurrentTask->pStackEnd);
    
    // Save previous task's stack pointer, if there was one
    if (pCurrentTask != NULL_PTR) {
        pCurrentTask->pStack = sp;
        // If previous task was running (not blocked), re-queue it
        if (pCurrentTask->state == APOS_TASK_RUNNING) {
            pCurrentTask->state = APOS_TASK_READY;
            APOS_INSERT_QUEUE(pCurrentTask);
        }
    }

    // Select next task from the head of the ready queue
    pCurrentTask = pHead;
    
    if (pCurrentTask != NULL_PTR) {
        // Remove the running task from the ready queue
        APOS_RemoveFromReadyQueue(pCurrentTask);
        pCurrentTask->state = APOS_TASK_RUNNING;

        if (pCurrentTask->TimeLeft < 1) {   // if timeslice has run reset the timeslice
            pCurrentTask->TimeLeft = pCurrentTask->TimeSlice;
        }
    } else {
        // This should not be reached if an idle task is implemented.
        while(1);
    }

    return pCurrentTask->pStack;
}

void APOS_Scheduler(void) {
    __asm volatile ("SVC 0");
}

void SVC_Handler(void) {
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void APOS_TaskDelay(uint32_t ticks) {
    APOS_EnterCriticalRegion();
    
    // Set the state to BLOCKED. APOS_Select_next_task will see this and will not re-queue the task.
    pCurrentTask->delay = ticks;
    pCurrentTask->state = APOS_TASK_BLOCKED;
    
    APOS_ExitCriticalRegion();
    
    APOS_Scheduler(); // Yield the CPU
}

void APOS_EnterCriticalRegion(void) {
    __disable_irq();
    criticalSectionCnt++;
    __enable_irq();
}

void APOS_ExitCriticalRegion(void) {
    __disable_irq();
    if (criticalSectionCnt > 0) {
        criticalSectionCnt--;
    }
    if (criticalSectionCnt == 0 && schedulerPending) {
        schedulerPending = FALSE;
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
    __enable_irq();
}

uint32_t APOS_GetStatusRegion() {
    return criticalSectionCnt;
}

void APOS_NOP(void) {
    while(1) {
    }
}

APOS_TCB_STRUCT* APOS_GetCurrentTask(void) {
    return pCurrentTask;
}

void APOS_SetSchedulerPending(void) {
    schedulerPending = TRUE;
}

void APOS_UpdateDelays(void) {
    APOS_EnterCriticalRegion();
    for(uint8_t i=0; i < APOS_TASK_NR; i++) {
        if(TCB_Tasks[i].state == APOS_TASK_BLOCKED) {
            if(TCB_Tasks[i].delay > 0) {
                TCB_Tasks[i].delay--;
            }
            if(TCB_Tasks[i].delay == 0) {
                TCB_Tasks[i].state = APOS_TASK_READY;
                // Task is now ready, insert it back into the ready queue
                APOS_INSERT_QUEUE(&TCB_Tasks[i]);
            }
        }
    }
    APOS_ExitCriticalRegion();
}

__attribute__((noinline, cold))
static void APOS_StackCorrupted(void)
{
    /* Stack guard violation: stop here */
    while (1) {
    }
}

static inline void APOS_AssertStackGuard(const void *pStack) {
    
    const uint32_t *p = (const uint32_t *)pStack;
    const uint32_t *g = (const uint32_t *)APOS_STACK_GUARD;

    if ((p[0] == g[0]) && (p[1] == g[1])) {
        return;
    }
    APOS_StackCorrupted();
}
