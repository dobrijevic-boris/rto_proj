/**
  ******************************************************************************
  * @file    main.c 
  * @author  Josef Langer
  * @version V2.0
  * @date    08.10.2022
  * @brief   Main Module, Initialization, Starting Point for Superloop / RTOS
  ******************************************************************************
  */
	
#include "StdDef.h"
#include "BSP/Key.h"
#include "BSP/Led.h"
#include "BSP/Adc.h"
#include "BSP/systick.h"
#include "stm32f0xx_gpio.h"
#include "BSP/TftDisplay.h"
#include "Fonts/TftFont_6x8.h"
#include "TaskAll.h"
#include "BSP/Debug.h"
#include "TestTask.h"

#include "APOS.h"

#include <stdio.h>


/* Private define ------------------------------------------------------------*/

#define TASK_PRIO (uint32_t)1
#define TASK_TMSLC (uint32_t)10



static uint32_t taskA_stack[APOS_TASK_STACK_SZ];
static uint32_t taskB_stack[APOS_TASK_STACK_SZ];
static uint32_t taskC_stack[APOS_TASK_STACK_SZ];

static void initStack(void) {
    for(uint32_t i=0; i<APOS_TASK_STACK_SZ;i++) {
        taskA_stack[i] = 0xAAAAAAAA;
    }    
    for(uint32_t i=0; i<APOS_TASK_STACK_SZ;i++) {
        taskB_stack[i] = 0xBBBBBBBB;
    }    
    for(uint32_t i=0; i<APOS_TASK_STACK_SZ;i++) {
        taskC_stack[i] = 0xCCCCCCCC;
    }
}

int main(void) {
    
    void __disable_irq();
    NVIC_SetPriority(PendSV_IRQn, 3); 
    
    Debug_Init();
    Tick_InitSysTick();
    
    APOS_Init();
    initStack();
    APOS_TASK_Create(&TCB_Tasks[TASK_A], "TaskA", TASK_PRIO, testTaskA, taskA_stack, APOS_TASK_STACK_SZ, TASK_TMSLC);
    APOS_TASK_Create(&TCB_Tasks[TASK_B], "TaskB", TASK_PRIO, testTaskB, taskB_stack, APOS_TASK_STACK_SZ, TASK_TMSLC);
    APOS_TASK_Create(&TCB_Tasks[TASK_C], "TaskC", TASK_PRIO, testTaskC, taskC_stack, APOS_TASK_STACK_SZ, TASK_TMSLC);
   
    void __enable_irq(void);
    
    APOS_Start();
    
    
    while(1) {
    }
}


/* #define NDEBUG to ignore all asserts */
#define ASSERT_BUFF_SIZE 150u
/**
 * @brief  Reports the name of the source file and the source line number
 *					where the assert error has occurred.
 * @param	expr: expression that resulted to false
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
*/
void __aeabi_assert(const char * expr, const char * file, int line)
{
  /* User can add his own implementation to report the expression, file name
	 and line number, ex:
	 printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  volatile uint32_t linev;
	/* Buffer size, because of long file names */
  volatile uint8_t tmpBuf[ASSERT_BUFF_SIZE];

	if(expr != NULL_PTR && file != NULL_PTR)
	{
		for (uint8_t i = 0; i < ASSERT_BUFF_SIZE; i++)
		{
			/* insert expression */
			if((*expr) != '\0')
			{
				tmpBuf[i] = *expr++;
			}
			/* insert filename */
			else if((*file) != '\0')
			{
				tmpBuf[i] = *file++;
			}
		}
	}
	linev = line;

  /* Infinite loop */
	/* Now Debug with debugger (tmpBuf, Linev) */
  while (1);
}
