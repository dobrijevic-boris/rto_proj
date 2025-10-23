/**
 ******************************************************************************
 * @file    Key.c
 * @author  Julian Grasböck
 * @version V1.0.0
 * @date    14.11.2017
 * @brief   Abstracts Key handling.
 * @copyright
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH
 ******************************************************************************
 */

#include "Key.h"
#include "stm32f0xx_gpio.h"

/****************************************************************
 * User functions
 ***************************************************************/

/** 
 * Function to initialize the given keys
 */
void Key_Init(void)
{
  /* Enable clock for key gpio ports */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  GPIO_InitTypeDef gpioInit;
  GPIO_StructInit(&gpioInit);
  
  /* Init USER0 and USER1 gpios */
  gpioInit.GPIO_Mode = GPIO_Mode_IN;
  gpioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOD, &gpioInit);
  
  /* Init WAKEUP gpios */
  gpioInit.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &gpioInit);
}

/** 
 * Function to get the current state of a key
 */
BOOL Key_GetState(KeyType const key)
{
  BitAction KeyGpioState = Bit_RESET;
  GPIO_TypeDef* GPIO_Port = GPIOA;
  uint16_t GPIO_Pin = GPIO_Pin_0;
  
  /* Determine key that should be read */
  switch(key)
  {
    case KeyType_WAKEUP:
      /* Nothing todo ! */
      /* Port and pin value are initial values */
      break;
    
    case KeyType_USER0:
      /* Set gpio port */
      GPIO_Port = GPIOD;
      /* Gpio pin is initial value */
      break;
    
    case KeyType_USER1:
      /* Set gpio pin and port */
      GPIO_Port = GPIOD;
      GPIO_Pin = GPIO_Pin_1;
      break;
    
    default:
      /* Incorrect key value given
         Default return FALSE
         TODO: Insert error handling */
      return FALSE;
  }
  
  /* Read gpio value */
  KeyGpioState = (BitAction)GPIO_ReadInputDataBit(GPIO_Port, GPIO_Pin);
  /* Determine and return key state */
  if(KeyGpioState == Bit_SET)
  {
    return TRUE;
  }
  return FALSE;
}
