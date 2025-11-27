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



static uint32_t taskStacks[APOS_TASK_NR][APOS_TASK_STACK_SZ];

static void initStack(void) {
    for(uint32_t j=0; j<APOS_TASK_NR;j++) {
        
        for(uint32_t i=0; i<APOS_TASK_STACK_SZ;i++) {
            taskStacks[j][i] = 0xFFFFFFFF - j;
        }
    }
}

int main(void) {
    
    __disable_irq();
    NVIC_SetPriority(PendSV_IRQn, 3); 
    
    initStack(); // init stack to 0xFF for simpler debugging
    Debug_Init();
    Tick_InitSysTick();
    
    Key_Init();
    Led_Init();
    Tft_Init();
    Tft_SetFont(&TftFont_6x8);
    Adc_Init(ADC_CHANNEL_POTENTIOMETER);
    Tft_ClearScreen();
    
    APOS_Init();
    APOS_TASK_Create(&TCB_Tasks[TASK_COUNTER], "Counter", TASK_PRIO, TaskCounter, taskStacks[TASK_COUNTER], APOS_TASK_STACK_SZ, TASK_TMSLC, APOS_TASK_RUNNING, 0);
    APOS_TASK_Create(&TCB_Tasks[TASK_KEY], "Key", TASK_PRIO, TaskKey, taskStacks[TASK_KEY], APOS_TASK_STACK_SZ, TASK_TMSLC, APOS_TASK_READY, 0);
    APOS_TASK_Create(&TCB_Tasks[TASK_LED], "Led", TASK_PRIO, TaskLed, taskStacks[TASK_LED], APOS_TASK_STACK_SZ, TASK_TMSLC, APOS_TASK_READY, 0);
    APOS_TASK_Create(&TCB_Tasks[TASK_MANDELBROT], "Mandelbrot", TASK_PRIO, TaskMandelbrot, taskStacks[TASK_MANDELBROT], APOS_TASK_STACK_SZ, TASK_TMSLC, APOS_TASK_READY, 0);
    APOS_TASK_Create(&TCB_Tasks[TASK_POTI], "Poti", TASK_PRIO, TaskPoti, taskStacks[TASK_POTI], APOS_TASK_STACK_SZ, TASK_TMSLC, APOS_TASK_READY, 0);
    APOS_TASK_Create(&TCB_Tasks[TASK_WATCH], "Watch", TASK_PRIO, TaskWatch, taskStacks[TASK_WATCH], APOS_TASK_STACK_SZ, TASK_TMSLC, APOS_TASK_READY, 0);
    APOS_TASK_Create(&TCB_Tasks[TASK_NOP], "NOP", TASK_PRIO, APOS_NOP, taskStacks[TASK_NOP], APOS_TASK_STACK_SZ, TASK_TMSLC, APOS_TASK_READY, 0);
   
    APOS_Start();
    __enable_irq();
    
    
    
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
