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

// All task states of TCB
typedef enum {
    APOS_TASK_READY=0,          // ready to run
    APOS_TASK_RUNNING,          // running task, only one
    APOS_TASK_BLOCKED,          // blocked (currently only by self -> yield)
    APOS_TASK_WAITING_EVENT,    // waiting (for mutex)
    APOS_TASK_SUSPENDED         // currently not in use
} APOS_TASK_STATE;

// Task Control Block
typedef struct APOS_TCB_STRUCT APOS_TCB_STRUCT;
struct APOS_TCB_STRUCT {
    char const* pTaskName;
    uint32_t Priority;                  // changes on inheritance
    uint32_t basePriority;              // stays the same after start
    void (*pRoutine)(void);
    void * pStack;
    void * pStackEnd;
    uint32_t StackSize;
    uint32_t TimeSlice;
    APOS_TASK_STATE state;
    uint32_t delay;                     // in ticks (1ms)
    uint32_t TimeLeft;                  // time left running in ticks (ms)
    APOS_TASKEVENT events;
    APOS_TASKEVENT waitForEvents;
    struct APOS_TCB_STRUCT* pNextRdy;   // pointer to next ready task
};

// Mutex "Control Block"
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

// @brief Saves the current task's registers to the stack
// @param uint32_t    *pStack Pointer to the current stack top
// @returns uint32_t* The new stack pointer after saving registers
extern uint32_t* APOS_save_regs(uint32_t *pStack);

// @brief Restores the task's registers from the stack
// @param uint32_t *pStack Pointer to the stack containing registers
extern void APOS_restore_regs(uint32_t *pStack);

// @brief Sets the process stack pointer (PSP) and jumps to the task's PC
// @param uint32_t pc The Program Counter to jump to
extern void APOS_set_ctrl_pc(uint32_t pc);

// ========== c functions   ==========

// @brief Initializes the Real-Time Operating System and hardware priorities
void APOS_Init (void); 

// @brief Starts the Real-Time Operating System and launches the first task
void APOS_Start (void); 

// @brief Triggers the scheduler to switch tasks (usually via SVC)
void APOS_Scheduler(void); 

// @brief Clears specific events from a task's event mask
// @param APOS_TCB_STRUCT* pTask Pointer to the task
// @returns APOS_TASKEVENT The events that were set before clearing
APOS_TASKEVENT APOS_ClearEvents(APOS_TCB_STRUCT* pTask);

// @brief Waits for specific events to be signaled
// @param APOS_TASKEVENT eventMask Bitmask of events to wait for
// @returns APOS_TASKEVENT The events that occurred
APOS_TASKEVENT APOS_WaitEvent(APOS_TASKEVENT eventMask);

// @brief Signals an event to a specific task, potentially waking it up
// @param APOS_TCB_STRUCT* pTask Pointer to the task to signal
// @param APOS_TASKEVENT event Event flags to set
void APOS_SignalEvent(APOS_TCB_STRUCT* pTask, APOS_TASKEVENT event);

// @brief Selects the next task to run from the ready queue
// @param uint32_t *sp Current stack pointer
// @returns uint32_t* The stack pointer of the task to run
uint32_t* APOS_Select_next_task(uint32_t *sp); 

// @brief Blocks the current task for a specified number of system ticks
// @param uint32_t ticks Number of ticks to delay
void APOS_TaskDelay(uint32_t ticks);

// @brief Inserts a task into the ready queue, sorted by priority
// @param APOS_TCB_STRUCT* pTask Pointer to the task to insert
void APOS_INSERT_QUEUE(APOS_TCB_STRUCT* pTask);

// @brief Helper function to insert a task into a sorted linked list
// @param APOS_TCB_STRUCT** ppHead Pointer to the head pointer of the list
// @param APOS_TCB_STRUCT* pTask Pointer to the task to insert
void APOS_List_Insert(APOS_TCB_STRUCT** ppHead, APOS_TCB_STRUCT* pTask);

// @brief Creates and initializes a new task
void APOS_TASK_Create ( 
    APOS_TCB_STRUCT* pTask, // @param APOS_TCB_STRUCT* pTask Pointer to the Task Control Block
    const char* pTaskName,  // @param const char* pTaskName Name of the task (for debug)
    uint32_t Priority,      // @param uint32_t Priority Priority of the task
    void (*pRoutine)(void), // @param void (*pRoutine)(void) Function pointer to the task routine
    void * pStack,          // @param void * pStack Pointer to the stack memory
    uint32_t StackSize,     // @param uint32_t StackSize Size of the stack
    uint32_t TimeSlice,     // @param uint32_t TimeSlice Time slice for Round Robin
    APOS_TASK_STATE state,  // @param APOS_TASK_STATE state Initial state of the task
    uint32_t delay,         // @param uint32_t delay Initial delay
    uint32_t TimeLeft);     // @param uint32_t TimeLeft Initial time left
    
// @brief Disables interrupts to enter a critical region
void APOS_EnterCriticalRegion(void);
    
// @brief Re-enables interrupts to exit a critical region
void APOS_ExitCriticalRegion(void);
    
// @brief Returns the current nesting level of the critical region
// @returns uint32_t The critical section nesting count
uint32_t APOS_GetStatusRegion();    
    
// @brief Idle task loop that does nothing
void APOS_NOP(void);
    
// @brief Gets the Task Control Block of the currently running task
// @returns APOS_TCB_STRUCT* Pointer to the current task's TCB
APOS_TCB_STRUCT* APOS_GetCurrentTask(void); 
    
// @brief Sets a flag indicating that the scheduler should run (pending)
void APOS_SetSchedulerPending(void);

// @brief Updates the delay counters of blocked tasks (called by SysTick)
void APOS_UpdateDelays(void);

// @brief Updates the time slice of the running task
void APOS_UpdateTimeLeft(void);

// --- Mutex functions ---
// @brief Initializes a mutex structure
// @param APOS_MUTEX* pMutex Pointer to the mutex to initialize
void APOS_MUTEX_Create(APOS_MUTEX* pMutex);

// @brief Inserts the current task into the mutex's wait queue
// @param APOS_MUTEX* pMutex Pointer to the mutex
void APOS_MUTEX_InsertQueue(APOS_MUTEX* pMutex);

// @brief Tries to lock a mutex, blocking if unavailable (implements priority inheritance)
// @param APOS_MUTEX* pMutex Pointer to the mutex to lock
void APOS_MUTEX_LockBlocked(APOS_MUTEX* pMutex);

// @brief Unlocks a mutex, potentially waking waiting tasks and restoring priority
// @param APOS_MUTEX* pMutex Pointer to the mutex to unlock
void APOS_MUTEX_Unlock(APOS_MUTEX* pMutex);

// @brief Removes the highest priority task from the mutex wait queue
// @param APOS_MUTEX* pMutex Pointer to the mutex
// @returns APOS_TCB_STRUCT* Pointer to the popped task
APOS_TCB_STRUCT* APOS_MUTEX_PopWaitingQueue(APOS_MUTEX* pMutex);

// @brief Wakes up the new owner of the mutex
// @param APOS_MUTEX* pMutex Pointer to the mutex
void APOS_MUTEX_WakeTask(APOS_MUTEX* pMutex);

#endif
