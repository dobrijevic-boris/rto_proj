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


/* Private define ------------------------------------------------------------*/
#define TASK_SCHEDULER_COUNTER  (uint32_t)20    // task counter max time between calls
#define TASK_SCHEDULER_KEY      (uint32_t)50    // task key max time between calls
#define TASK_SCHEDULER_LED      (uint32_t)100    // task led max time between calls
#define TASK_SCHEDULER_WATCH    (uint32_t)1000  // task watch max time between calls
#define TASK_SCHEDULER_POTI     (uint32_t)50    // task poti max time between calls


int main(void) {
    
    Key_Init();
    Led_Init();
    Tft_Init();
    Tft_SetFont(&TftFont_16x24);
    Adc_Init(ADC_CHANNEL_POTENTIOMETER);	
    Tick_InitSysTick();
    Debug_Init();
    
    BOOL mandelbrotFinished = FALSE;
    while (1) {
        static uint32_t lastCounter = 0;
        static uint32_t lastKey = 0;
        static uint32_t lastPoti = 0;
        static uint32_t lastWatch = 0;
        static uint32_t lastLed = 0;

        static uint32_t lastTick = 0;
        uint32_t now = SysTick_GetTicks();
        uint32_t delta = now - lastTick;
        lastTick = now;

        // Zeitaufsummierung
        lastCounter += delta;
        lastKey     += delta;
        lastPoti    += delta;
        lastWatch   += delta;
        lastLed     += delta;

        if (lastCounter >= TASK_SCHEDULER_COUNTER) {
            Debug_SwitchDebugPin(DEBUG_PIN_TASKCOUNTER, Bit_SET);
            TaskCounter();
            Debug_SwitchDebugPin(DEBUG_PIN_TASKCOUNTER, Bit_RESET);
            lastCounter = 0;
        }
        else if (lastKey >= TASK_SCHEDULER_KEY) {
            Debug_SwitchDebugPin(DEBUG_PIN_TASKKEY, Bit_SET);
            TaskKey();
            Debug_SwitchDebugPin(DEBUG_PIN_TASKKEY, Bit_RESET);
            lastKey = 0;
        }
        else if (lastPoti >= TASK_SCHEDULER_POTI) {
            Debug_SwitchDebugPin(DEBUG_PIN_TASKPOTI, Bit_SET);
            TaskPoti();
            Debug_SwitchDebugPin(DEBUG_PIN_TASKPOTI, Bit_RESET);
            lastPoti = 0;
        }
        else if (lastWatch >= TASK_SCHEDULER_WATCH) {
            Debug_SwitchDebugPin(DEBUG_PIN_TASKWATCH, Bit_SET);
            TaskWatch();
            Debug_SwitchDebugPin(DEBUG_PIN_TASKWATCH, Bit_RESET);
            lastWatch = 0;
        }
        else if (lastLed >= TASK_SCHEDULER_LED) {
            Debug_SwitchDebugPin(DEBUG_PIN_TASKLED, Bit_SET);
            TaskLed();
            Debug_SwitchDebugPin(DEBUG_PIN_TASKLED, Bit_RESET);
            lastLed = 0;
        }
        else {
            // kein anderer Task fällig ? Mandelbrot
            Debug_SwitchDebugPin(DEBUG_PIN_TASKMANDELBROT, Bit_SET);
            mandelbrotFinished = TaskMandelbrot();
            Debug_SwitchDebugPin(DEBUG_PIN_TASKMANDELBROT, Bit_RESET);

            if (mandelbrotFinished) {
                Tft_ClearScreen();
            }
        }
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
