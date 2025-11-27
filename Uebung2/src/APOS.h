#ifndef APOS_H
#define APOS_H

#include <stdint.h>
#define APOS_TASK_STACK_SZ (uint32_t)256


// enum for tasks
typedef enum {
    TASK_COUNTER = 0,
    TASK_KEY,
    TASK_LED,
    TASK_MANDELBROT,
    TASK_POTI,
    TASK_WATCH,
    TASK_NOP,
    APOS_TASK_NR
} APOS_TASK_ID;

typedef enum {
    APOS_TASK_READY=0,
    APOS_TASK_RUNNING,
    APOS_TASK_BLOCKED,
    APOS_TASK_SUSPENDED
} APOS_TASK_STATE;

typedef struct {
    char const* pTaskName;
    uint32_t Priority;
    void (*pRoutine)(void);
    void * pStack;
    uint32_t StackSize;
    uint32_t TimeSlice;
    APOS_TASK_STATE state;
    uint32_t delay; // in ticks (1ms)
} APOS_TCB_STRUCT;


extern APOS_TCB_STRUCT TCB_Tasks[];

// asm functions
extern uint32_t* APOS_save_regs(uint32_t *pStack);
extern void APOS_restore_regs(uint32_t *pStack);
extern void APOS_set_ctrl_pc(uint32_t pc);



void APOS_Init (void); // Initialisert das Echtzeitbetriebssystem
void APOS_Start (void); // Starten des Echtzeitbetriebssystems
void APOS_Scheduler(void); // Call SVC
uint32_t* APOS_Select_next_task(uint32_t *sp); // select the next task to run
void APOS_TaskDelay(uint32_t ticks);
void APOS_TASK_Create ( 
    APOS_TCB_STRUCT* pTask, // TaskControlBlock
    const char* pTaskName, // Task Name – nur für Debug-Zwecke
    uint32_t Priority, // Priorität des Tasks (vorerst nicht in Verwendung)
    void (*pRoutine)(void), // Startadresse Task (ROM)
    void * pStack, // Startadresse Stack des Tasks (RAM)
    uint32_t StackSize, // Größe des Stacks
    uint32_t TimeSlice, // Time-Slice für Round Robin Scheduling
    APOS_TASK_STATE state,
    uint32_t delay);
void APOS_EnterCriticalRegion(void);
void APOS_ExitCriticalRegion(void);
uint32_t APOS_GetStatusRegion();    
    void APOS_NOP(void);
#endif
