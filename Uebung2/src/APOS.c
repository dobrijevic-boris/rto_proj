#include "APOS.h"


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
    
}
