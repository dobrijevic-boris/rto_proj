#ifndef APOS_H
#define APOS_H

#include <stdint.h>
#define APOS_TASK_STACK_SZ (uint32_t)256

typedef uint32_t APOS_TASKEVENT;
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
    APOS_TASK_WAITING_EVENT,
    APOS_TASK_SUSPENDED
} APOS_TASK_STATE;

typedef struct APOS_TCB_STRUCT APOS_TCB_STRUCT;
struct APOS_TCB_STRUCT {
    char const* pTaskName;
    uint32_t Priority;
    uint32_t basePriority;
    void (*pRoutine)(void);
    void * pStack;
    void * pStackEnd;
    uint32_t StackSize;
    uint32_t TimeSlice;
    APOS_TASK_STATE state;
    uint32_t delay; // in ticks (1ms)
    uint32_t TimeLeft; // time left running in ticks (ms)
    APOS_TASKEVENT events;
    APOS_TASKEVENT waitForEvents;
    struct APOS_TCB_STRUCT* pNextRdy;
};

typedef struct {
    uint8_t counter;
    APOS_TCB_STRUCT* pOwner;
    APOS_TCB_STRUCT* pWaitList;
}APOS_MUTEX;
// --- external globals
extern APOS_TCB_STRUCT TCB_Tasks[];
extern APOS_TCB_STRUCT* pHead;
extern uint8_t systemInit;


// ========== asm functions ==========
extern uint32_t* APOS_save_regs(uint32_t *pStack);

extern void APOS_restore_regs(uint32_t *pStack);


// ========== c functions   ==========
// Initialisert das Echtzeitbetriebssystem
void APOS_Init (void); 

// Starten des Echtzeitbetriebssystems
void APOS_Start (void); 

// Call SVC
void APOS_Scheduler(void); 

APOS_TASKEVENT APOS_ClearEvents(APOS_TCB_STRUCT* pTask);
APOS_TASKEVENT APOS_WaitEvent(APOS_TASKEVENT eventMask);
void APOS_SignalEvent(APOS_TCB_STRUCT* pTask, APOS_TASKEVENT event);
// select the next task to run
uint32_t* APOS_Select_next_task(uint32_t *sp); 

void APOS_TaskDelay(uint32_t ticks);

// inserts task into ready queue
void APOS_INSERT_QUEUE(APOS_TCB_STRUCT* pTask);

// inserts task into a sorted list
void APOS_List_Insert(APOS_TCB_STRUCT** ppHead, APOS_TCB_STRUCT* pTask);

void APOS_TASK_Create ( 
    APOS_TCB_STRUCT* pTask, // TaskControlBlock
    const char* pTaskName, // Task Name nur fuer DebugZwecke
    uint32_t Priority, // Prioritaet des Tasks (kann kurzzeitig durch Mutex erhoeht werden)
    void (*pRoutine)(void), // Startadresse Task (ROM)
    void * pStack, // Startadresse Stack des Tasks (RAM)
    uint32_t StackSize, // Groesse des Stacks
    uint32_t TimeSlice, // Time Slice fuer Round Robin Scheduling
    APOS_TASK_STATE state,
    uint32_t delay,
    uint32_t TimeLeft);
    
void APOS_EnterCriticalRegion(void);
    
void APOS_ExitCriticalRegion(void);
    
uint32_t APOS_GetStatusRegion();    
    
void APOS_NOP(void);
    
APOS_TCB_STRUCT* APOS_GetCurrentTask(void); 
    
void APOS_SetSchedulerPending(void);

void APOS_UpdateDelays(void);

void APOS_UpdateTimeLeft(void);

// Mutex functions
void APOS_MUTEX_Create(APOS_MUTEX* pMutex);

void APOS_MUTEX_InsertQueue(APOS_MUTEX* pMutex);

void APOS_MUTEX_LockBlocked(APOS_MUTEX* pMutex);

void APOS_MUTEX_Unlock(APOS_MUTEX* pMutex);

APOS_TCB_STRUCT* APOS_MUTEX_PopWaitingQueue(APOS_MUTEX* pMutex);

void APOS_MUTEX_WakeTask(APOS_MUTEX* pMutex);

#endif
    