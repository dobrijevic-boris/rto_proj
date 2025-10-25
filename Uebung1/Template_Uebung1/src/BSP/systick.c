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
#include "Debug.h"

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
  Debug_SwitchDebugPin(DEBUG_PIN_SYSTICK, Bit_SET);
  msTicks++;                                    // increment Tick-counter
  Debug_SwitchDebugPin(DEBUG_PIN_SYSTICK, Bit_RESET);
}

/*-------------------------------------------------------------------------------
* TICK_InitSysTick: Initializes System-Timer Interrupt to 1ms
* Parameters: (none)
* Return:     (none)
*------------------------------------------------------------------------------*/
void Tick_InitSysTick(void){
	SysTick_Config(SystemCoreClock/1000);      /* Generate interrupt each 1 ms  */
}

/*-----------------------------------------------------------------------------
* SysTick_GetmsTicks: Getter function for msTicks
* Parameters: (none)
* Return:     (uint32_t msTicks)
*----------------------------------------------------------------------------*/
uint32_t SysTick_GetTicks(void) {
  return msTicks;
}

