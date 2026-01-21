#ifndef __TICK_H
#define __TICK_H
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
// @brief Delays execution for a specified number of ticks
// @param uint32_t dlyTicks Number of ticks to delay
void 						Tick_Delay (uint32_t dlyTicks);

// @brief Initializes the SysTick timer
void						Tick_InitSysTick(void);

// @brief Handles the SysTick interrupt
void 						SysTick_Handler2 (void);

// @brief Gets the current tick count
// @returns uint32_t Current number of ticks
uint32_t                    SysTick_GetTicks(void);

#endif /* __TICK_H */
