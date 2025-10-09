/**
  ******************************************************************************
  * @file    systick.c 
  * @author  Josef Langer
  * @version V1.0
  * @date    24.11.2017
  * @brief   SysTick Timer Handling, 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/	
#include <stdint.h>
#include "stm32f0xx.h"
#include "systick.h"

/* Private define ------------------------------------------------------------*/
#define _1_sec			1000
#define _100_ms			100

/* Private variables ------------------------------------------------------- */
static uint32_t msTicks;      		// Counts 1ms timeTicks

/*-----------------------------------------------------------------------------
* SysTick_Handler: Counts every 1ms
* Parameters: (none)
* Return:     (none)
*----------------------------------------------------------------------------*/
void SysTick_Handler (void)  {
  msTicks++;                                    // increment Tick-counter
}

/*-------------------------------------------------------------------------------
* TICK_InitSysTick: Initializes System-Timer Interrupt to 1ms
* Parameters: (none)
* Return:     (none)
*------------------------------------------------------------------------------*/
void Tick_InitSysTick(void){
	SysTick_Config(SystemCoreClock/1000);      /* Generate interrupt each 1 ms  */
}
