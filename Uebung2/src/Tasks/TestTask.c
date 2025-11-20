#include "TestTask.h"
#include "APOS.h"
#include "BSP/Debug.h"
#include "stm32f0xx.h"

static void Debug_TaskOn_A(void);
static void Debug_TaskOff_A(void);
static void Debug_TaskOn_B(void);
static void Debug_TaskOff_B(void);
static void Debug_TaskOn_C(void);
static void Debug_TaskOff_C(void);


void FillTaskA(void)
{
    __asm volatile(
        "movs r0,  #0xA8\n"
        "mov  r8,  r0\n"
        "movs r0,  #0xA9\n"
        "mov  r9,  r0\n"
        "movs r0,  #0xAA\n"
        "mov  r10, r0\n"
        "movs r0,  #0xAB\n"
        "mov  r11, r0\n"
        "movs r0,  #0xAC\n"
        "mov  r12, r0\n"
        "movs r0,  #0xA0\n"
        "movs r1,  #0xA1\n"
        "movs r2,  #0xA2\n"
        "movs r3,  #0xA3\n"
        "movs r4,  #0xA4\n"
        "movs r5,  #0xA5\n"
        "movs r6,  #0xA6\n"
        "movs r7,  #0xA7\n"
    );
}

void FillTaskB(void)
{
    __asm volatile (
        "movs r0,  #0xB8\n"
        "mov  r8,  r0\n"
        "movs r0,  #0xB9\n"
        "mov  r9,  r0\n"
        "movs r0,  #0xBA\n"
        "mov  r10, r0\n"
        "movs r0,  #0xBB\n"
        "mov  r11, r0\n"
        "movs r0,  #0xBC\n"
        "mov  r12, r0\n"
        "movs r0,  #0xB0\n"
        "movs r1,  #0xB1\n"
        "movs r2,  #0xB2\n"
        "movs r3,  #0xB3\n"
        "movs r4,  #0xB4\n"
        "movs r5,  #0xB5\n"
        "movs r6,  #0xB6\n"
        "movs r7,  #0xB7\n"
    );
}

void FillTaskC(void)
{
    __asm volatile (
        "movs r0,  #0xC8\n"
        "mov  r8,  r0\n"
        "movs r0,  #0xC9\n"
        "mov  r9,  r0\n"
        "movs r0,  #0xCA\n"
        "mov  r10, r0\n"
        "movs r0,  #0xCB\n"
        "mov  r11, r0\n"
        "movs r0,  #0xCC\n"
        "mov  r12, r0\n"
        "movs r0,  #0xC8\n"
        "mov  r8,  r0\n"
        "movs r0,  #0xC0\n"
        "movs r1,  #0xC1\n"
        "movs r2,  #0xC2\n"
        "movs r3,  #0xC3\n"
        "movs r4,  #0xC4\n"
        "movs r5,  #0xC5\n"
        "movs r6,  #0xC6\n"
        "movs r7,  #0xC7\n"
    );
}
void testTaskA(void) {
   FillTaskA();
    while (1) {
    Debug_TaskOn_A();
    // enter Code
    Debug_TaskOff_A();
    APOS_Scheduler();
} 
}
void testTaskB(void) {
    FillTaskB();
    while (1) {
    Debug_TaskOn_B();
    // enter Code
    Debug_TaskOff_B();
    APOS_Scheduler();
    }
}
void testTaskC(void) {
    FillTaskC();
    while (1) {
    Debug_TaskOn_C();
    // enter Code
    Debug_TaskOff_C();
    APOS_Scheduler();
    }
}


static void Debug_TaskOn_A(void) {
    Debug_SwitchDebugPin(DEBUG_PIN_TASKA, Bit_SET);    
}
static void Debug_TaskOn_B(void) {
    Debug_SwitchDebugPin(DEBUG_PIN_TASKB, Bit_SET);    
}
static void Debug_TaskOn_C(void) {
    Debug_SwitchDebugPin(DEBUG_PIN_TASKC, Bit_SET);    
}

static void Debug_TaskOff_A(void) {
    Debug_SwitchDebugPin(DEBUG_PIN_TASKA, Bit_RESET);    
}
static void Debug_TaskOff_B(void) {
    Debug_SwitchDebugPin(DEBUG_PIN_TASKB, Bit_RESET);    
}
static void Debug_TaskOff_C(void) {
    Debug_SwitchDebugPin(DEBUG_PIN_TASKC, Bit_RESET);    
}