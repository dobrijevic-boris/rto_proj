#include "APOS.h"

#include "stm32f0xx.h"

APOS_TCB_STRUCT TCB_Tasks[APOS_TASK_NR];

static uint8_t currentTask = TASK_COUNTER;
static volatile uint32_t criticalSectionCnt = 0;
// function prototypes
uint32_t* APOS_Select_next_task(uint32_t *sp);


void APOS_Init (void) 
{
    
}
void APOS_Start (void) 
{
    TCB_Tasks[currentTask].pStack =  (uint32_t*)TCB_Tasks[currentTask].pStack + 16;
    
    uint32_t pc = *((uint32_t*)TCB_Tasks[currentTask].pStack-2);
    // switch to currentTask
    __set_PSP((uint32_t)TCB_Tasks[currentTask].pStack);  // PSP zeigt ans Ende des Arrays
    APOS_set_ctrl_pc(pc);

}
void APOS_TASK_Create ( 
    APOS_TCB_STRUCT* pTask, // TaskControlBlock
    const char* pTaskName,  // Task Name – nur für Debug-Zwecke
    uint32_t Priority,      // Priorität des Tasks (vorerst nicht in Verwendung)
    void (*pRoutine)(void), // Startadresse Task (ROM)
    void * pStack,          // Startadresse Stack des Tasks (RAM)
    uint32_t StackSize,     // Größe des Stacks
    uint32_t TimeSlice,     // Time-Slice für Round Robin Scheduling  
    APOS_TASK_STATE state,  // task state (Ready, running, ...
    uint32_t delay)         // time delay in ticks (1ms)
    
    
{
    pTask->pTaskName = pTaskName;
    pTask->Priority = Priority;
    pTask->pRoutine = pRoutine;
    pTask->StackSize = StackSize;
    pTask->TimeSlice = TimeSlice;
    pTask->state = state;
    pTask->delay = delay;
    
    // Init stack
    uint32_t* sp = (uint32_t*)pStack;
    sp += APOS_TASK_STACK_SZ;
    *(--sp) = 0x01000000;
    *(--sp) = (uint32_t)pRoutine;
    *(--sp) = 0xFFFFFFFD; // lr (value doesnt mather)
    *(--sp) = 0xCCCCCCCC; // r12
    *(--sp) = 0x33333333; // r3
    *(--sp) = 0x22222222; // r2
    *(--sp) = 0x11111111; // r1
    *(--sp) = 0x00000000; // r0
   
    *(--sp) = 0xBBBBBBBB; // r11
    *(--sp) = 0xAAAAAAAA; // r10
    *(--sp) = 0x99999999; // r9
    *(--sp) = 0x88888888; // r8
    *(--sp) = 0x77777777; // r7
    *(--sp) = 0x66666666; // r6
    *(--sp) = 0x55555555; // r5
    *(--sp) = 0x44444444; // r4 
    pTask->pStack = sp;
}

uint32_t* APOS_Select_next_task(uint32_t *sp) {
    TCB_Tasks[currentTask].pStack = sp; // update sp after saving regs
    
    // set running task to ready
    if (TCB_Tasks[currentTask].state == APOS_TASK_RUNNING)
    {
        TCB_Tasks[currentTask].state = APOS_TASK_READY;
    }
    uint8_t tempTask = currentTask;
    uint8_t found = 0;
    do {
        // check if any task ready, if not run nop task
        currentTask = (currentTask + 1) % TASK_NOP;
        if(TCB_Tasks[currentTask].state == APOS_TASK_READY) {
            found = 1;
            break;
        }
    } while (currentTask != tempTask);
    if(!found) {
        currentTask = TASK_NOP;
    }
    
    // Mark new task as RUNNING
    TCB_Tasks[currentTask].state = APOS_TASK_RUNNING;
    
    uint32_t* old_pStack = (uint32_t*)TCB_Tasks[currentTask].pStack; // save old pStack for register load
    TCB_Tasks[currentTask].pStack = (uint32_t*)TCB_Tasks[currentTask].pStack + 8; // r4-r11
    return old_pStack; // return sp from new task
}


void APOS_Scheduler(void) {
    __asm volatile ("SVC 0");
}

void SVC_Handler(void) {
    SCB->ICSR = SCB->ICSR | (1<<28);
}

void APOS_TaskDelay(uint32_t ticks) {
    TCB_Tasks[currentTask].delay = ticks;
    TCB_Tasks[currentTask].state = APOS_TASK_BLOCKED;
    APOS_Scheduler();
}

void APOS_EnterCriticalRegion(void)
{
    // disable irq for atomic inc
    __disable_irq();
    criticalSectionCnt++;
    __enable_irq();
}

void APOS_ExitCriticalRegion(void)
{
    __disable_irq();
    if (criticalSectionCnt > 0) {
        criticalSectionCnt--;
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