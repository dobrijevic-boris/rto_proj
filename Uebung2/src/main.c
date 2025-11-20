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
#define TASK_STACK_SZ (uint32_t)1024

static uint32_t taskA_stack[TASK_STACK_SZ];
static uint32_t taskB_stack[TASK_STACK_SZ];
static uint32_t taskC_stack[TASK_STACK_SZ];



int main(void) {
    
    __set_PSP((uint32_t)(taskA_stack + TASK_STACK_SZ));  // PSP zeigt ans Ende des Arrays
    __set_CONTROL(0x02);                    // Bit 1 = 1 ? benutze PSP im Thread-Modus
    __ISB();     
    
    
    Debug_Init();
    Tick_InitSysTick();
    
    APOS_Init();
    
    FillTaskA();
    FillTaskB();
    FillTaskC();
    
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
