#ifndef APOS_H
#define APOS_H

#include <stdint.h>

#define APOS_TASK_STACK_SZ (uint32_t)512
// global static for psp
static uint32_t taskAStack[APOS_TASK_STACK_SZ];
static uint32_t taskBStack[APOS_TASK_STACK_SZ];
static uint32_t taskCStack[APOS_TASK_STACK_SZ];



typedef struct {
    char const* pTaskName;
    uint32_t Priority;
    void (*pRoutine)(void);
    void * pStack;
    uint32_t StackSize;
    uint32_t TimeSlice;
} APOS_TCB_STRUCT;


extern APOS_TCB_STRUCT TCB_TaskA;
extern APOS_TCB_STRUCT TCB_TaskB;
extern APOS_TCB_STRUCT TCB_TaskC;





void APOS_Init (void); // Initialisert das Echtzeitbetriebssystem
void APOS_Start (void); // Starten des Echtzeitbetriebssystems
void APOS_Scheduler(void);

void APOS_TASK_Create ( 
    APOS_TCB_STRUCT* pTask, // TaskControlBlock
    const char* pTaskName, // Task Name – nur für Debug-Zwecke
    uint32_t Priority, // Priorität des Tasks (vorerst nicht in Verwendung)
    void (*pRoutine)(void), // Startadresse Task (ROM)
    void * pStack, // Startadresse Stack des Tasks (RAM)
    uint32_t StackSize, // Größe des Stacks
    uint32_t TimeSlice // Time-Slice für Round Robin Scheduling
);


#endif
