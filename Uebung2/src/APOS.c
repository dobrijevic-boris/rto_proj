#include "APOS.h"

#include "stm32f0xx.h"

APOS_TCB_STRUCT TCB_Tasks[APOS_TASK_NR];


// function prototypes
void APOS_Scheduler(void);





void APOS_Init (void) 
{
    for(uint32_t i=0;i<APOS_TASK_STACK_SZ;i++) {
        taskAStack[i] = 0xFF;
    }
    for(uint32_t i=0;i<APOS_TASK_STACK_SZ;i++) {
        taskBStack[i] = 0xFF;
    }
    
    for(uint32_t i=0;i<APOS_TASK_STACK_SZ;i++) {
        taskCStack[i] = 0xFF;
    }
    
}
void APOS_Start (void) 
{
    // switch to taskA
    __set_PSP((uint32_t)TCB_Tasks[TASK_A].pStack);  // PSP zeigt ans Ende des Arrays
    __set_CONTROL(0x02);                    // Bit 1 = 1 ? benutze PSP im Thread-Modus
    __ISB();                                // Pipeline leeren
    __asm volatile (
        "ldr  r0, =0xFFFFFFFD \n"   // EXC_RETURN: thread mode, use PSP
        "mov  lr, r0          \n"
        "bx   lr              \n"   // pops HW frame from PSP, jumps to Task A
    );
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
    pStack += APOS_TASK_STACK_SZ;
    uint32_t* sp = (uint32_t*)pStack;
    *(--sp) = 0x01000000;
    *(--sp) = (uint32_t)pRoutine;
    *(--sp) = 0xFFFFFFFD; // lr (value doesnt mather)
    *(--sp) = 0xCCCCCCCC; // r12
    *(--sp) = 0x33333333; // r3
    *(--sp) = 0x22222222; // r2
    *(--sp) = 0x11111111; // r1
    *(--sp) = 0x00000000; // r0
/*    
    *(--sp) = 0xBBBBBBBB; // r11
    *(--sp) = 0xAAAAAAAA; // r10
    *(--sp) = 0x99999999; // r9
    *(--sp) = 0x88888888; // r8
    *(--sp) = 0x77777777; // r7
    *(--sp) = 0x66666666; // r6
    *(--sp) = 0x55555555; // r5
    *(--sp) = 0x44444444; // r4
  */  
    pTask->pStack = sp;
}

void APOS_Scheduler(void) {
    
    // activate PENDSV
    SCB->ICSR = SCB->ICSR | (1<<28);
}
