#ifndef DEBUG_H
#define DEBUG_H

#include "stm32f0xx_gpio.h"

#define DEBUG_PIN_SYSTICK GPIO_Pin_0
#define DEBUG_PIN_TASKCOUNTER GPIO_Pin_1
#define DEBUG_PIN_TASKKEY GPIO_Pin_3
#define DEBUG_PIN_TASKLED GPIO_Pin_12
#define DEBUG_PIN_TASKWATCH GPIO_Pin_13
#define DEBUG_PIN_TASKPOTI GPIO_Pin_14
#define DEBUG_PIN_TASKMANDELBROT GPIO_Pin_15

#define DEBUG_PIN_DEFAULT GPIO_Pin_0

#define DEBUG_PIN_TASKA GPIO_Pin_1
#define DEBUG_PIN_TASKB GPIO_Pin_3
#define DEBUG_PIN_TASKC GPIO_Pin_0


void Debug_Init(void);

void Debug_SwitchDebugPin(uint16_t const GPIO_Pin, BitAction const action);


#endif