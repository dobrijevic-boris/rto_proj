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
#include "Fonts/TftFont_16x24.h"
#include "TaskAll.h"
#include "BSP/Debug.h"

#include <stdio.h>

int main(void)
{
  Key_Init();
  Led_Init();
  Tft_Init();
  Tft_SetFont(&TftFont_16x24);
  Adc_Init(ADC_CHANNEL_POTENTIOMETER);	
  Tick_InitSysTick();
	Debug_Init();
  
  for(uint16_t i=0;i<1000;i++)
    {
      
      Debug_SwitchDebugPin(DEBUG_PIN_TASKLED, Bit_SET);
      Debug_SwitchDebugPin(DEBUG_PIN_TASKLED, Bit_RESET);
    }
  while (1)
  {
		Tft_ClearScreen();
    Debug_SwitchDebugPin(DEBUG_PIN_TASKCOUNTER, Bit_SET);
		TaskCounter();
    Debug_SwitchDebugPin(DEBUG_PIN_TASKCOUNTER, Bit_RESET);
    Debug_SwitchDebugPin(DEBUG_PIN_TASKKEY, Bit_SET);
		TaskKey();
    Debug_SwitchDebugPin(DEBUG_PIN_TASKKEY, Bit_RESET);
    Debug_SwitchDebugPin(DEBUG_PIN_TASKLED, Bit_SET);
		TaskLed();	
    Debug_SwitchDebugPin(DEBUG_PIN_TASKLED, Bit_RESET);
    Debug_SwitchDebugPin(DEBUG_PIN_TASKWATCH, Bit_SET);
		TaskWatch();
    Debug_SwitchDebugPin(DEBUG_PIN_TASKWATCH, Bit_RESET);
    Debug_SwitchDebugPin(DEBUG_PIN_TASKPOTI, Bit_SET);
		TaskPoti();	
    Debug_SwitchDebugPin(DEBUG_PIN_TASKPOTI, Bit_RESET);
    Debug_SwitchDebugPin(DEBUG_PIN_TASKMANDELBROT, Bit_SET);
		TaskMandelbrot();		
    Debug_SwitchDebugPin(DEBUG_PIN_TASKMANDELBROT, Bit_RESET);
    

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
