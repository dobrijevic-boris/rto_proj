#include "APOS.h"

#include "stm32f0xx.h"



APOS_TCB_STRUCT TCB_TaskA;
APOS_TCB_STRUCT TCB_TaskB;
APOS_TCB_STRUCT TCB_TaskC;

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
    __set_PSP((uint32_t)TCB_TaskA.pStack + TCB_TaskA.StackSize);  // PSP zeigt ans Ende des Arrays
    __set_CONTROL(0x02);                    // Bit 1 = 1 ? benutze PSP im Thread-Modus
    __ISB();                                // Pipeline leeren
    
    
}
void APOS_TASK_Create ( 
    APOS_TCB_STRUCT* pTask, // TaskControlBlock
    const char* pTaskName, // Task Name – nur für Debug-Zwecke
    uint32_t Priority, // Priorität des Tasks (vorerst nicht in Verwendung)
    void (*pRoutine)(void), // Startadresse Task (ROM)
    void * pStack, // Startadresse Stack des Tasks (RAM)
    uint32_t StackSize, // Größe des Stacks
    uint32_t TimeSlice) // Time-Slice für Round Robin Scheduling  
{
    pTask->pTaskName = pTaskName;
    pTask->Priority = Priority;
    pTask->pRoutine = pRoutine;
    pTask->pStack = pStack;
    pTask->StackSize = StackSize;
    pTask->TimeSlice = TimeSlice;
    
}

void APOS_Scheduler(void) {
    
    // activate PENDSV
    SCB->ICSR = SCB->ICSR | (1<<28);
}
