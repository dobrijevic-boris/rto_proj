#ifndef APOS_H
#define APOS_H

#include <stdint.h>
#define APOS_TASK_STACK_SZ (uint32_t)32
#define APOS_TASK_NR       (uint32_t)3


// enum for tasks
typedef enum {
    TASK_A = 0,
    TASK_B,
    TASK_C
} TTASKS;


typedef struct {
    char const* pTaskName;
    uint32_t Priority;
    void (*pRoutine)(void);
    void * pStack;
    uint32_t StackSize;
    uint32_t TimeSlice;
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
