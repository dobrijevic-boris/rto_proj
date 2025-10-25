
#include "Debug.h"
#include "stm32f0xx_gpio.h"

void Debug_Init(void) {
    
    
    /* Enable clock for DEBUG gpio port registers */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    /* Init all DebugPorts */
    GPIO_InitTypeDef gpioInit;    
    GPIO_StructInit(&gpioInit);
    

    gpioInit.GPIO_Mode = GPIO_Mode_OUT;
    gpioInit.GPIO_OType = GPIO_OType_PP;
    
    gpioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                        GPIO_Pin_3 | GPIO_Pin_12 |
                        GPIO_Pin_13 | GPIO_Pin_14 | 
                        GPIO_Pin_15;
    GPIO_Init(GPIOB, &gpioInit);
}


void Debug_SwitchDebugPin(uint16_t const GPIO_Pin, BitAction const action) {
    
  GPIO_WriteBit(GPIOB, GPIO_Pin, action);
}
