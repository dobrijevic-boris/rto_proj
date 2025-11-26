#include "APOS.h"

#include "stm32f0xx.h"

APOS_TCB_STRUCT TCB_Tasks[APOS_TASK_NR];

static uint8_t state = TASK_A;

// function prototypes
uint32_t* APOS_Select_next_task(uint32_t *sp);


void APOS_Init (void) 
{
    
}
void APOS_Start (void) 
{
    TCB_Tasks[TASK_A].pStack =  (uint32_t*)TCB_Tasks[TASK_A].pStack + 16;
    
    uint32_t pc = *((uint32_t*)TCB_Tasks[TASK_A].pStack-2);
    // switch to taskA
    __set_PSP((uint32_t)TCB_Tasks[TASK_A].pStack);  // PSP zeigt ans Ende des Arrays
    APOS_set_ctrl_pc(pc);

}
void APOS_TASK_Create ( 
    APOS_TCB_STRUCT* pTask, // TaskControlBlock
    const char* pTaskName,  // Task Name – nur für Debug-Zwecke
    uint32_t Priority,      // Priorität des Tasks (vorerst nicht in Verwendung)
    void (*pRoutine)(void), // Startadresse Task (ROM)
    void * pStack,          // Startadresse Stack des Tasks (RAM)
    uint32_t StackSize,     // Größe des Stacks
    uint32_t TimeSlice)     // Time-Slice für Round Robin Scheduling  
{
    pTask->pTaskName = pTaskName;
    pTask->Priority = Priority;
    pTask->pRoutine = pRoutine;
    pTask->StackSize = StackSize;
    pTask->TimeSlice = TimeSlice;
    
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
    TCB_Tasks[state].pStack = sp; // update sp after saving regs
    switch(state)
    {
        case TASK_A:
            state = TASK_B;
            break;
        case TASK_B:
            state = TASK_C;
            break;
        case TASK_C:
            state = TASK_A;
            break;
        default:
            state = TASK_A;
            break;
    }
    uint32_t* old_pStack = (uint32_t*)TCB_Tasks[state].pStack; // save old pStack for register load
    TCB_Tasks[state].pStack = (uint32_t*)TCB_Tasks[state].pStack + 8; // r4-r11
    return old_pStack; // return sp from new task
}


void APOS_Scheduler(void) {
    __asm volatile ("SVC 0");
}

void SVC_Handler(void) {
    SCB->ICSR = SCB->ICSR | (1<<28);
}
