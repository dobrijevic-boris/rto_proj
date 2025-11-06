/**
 ******************************************************************************
 * @file    Led.h
 * @author  Julian Grasböck
 * @version V1.0.0
 * @date    14.11.2017
 * @brief   Abstracts Led handling.
 * @copyright
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, 
 ******************************************************************************
 */

#include "Led.h"
#include "stm32f0xx_gpio.h"


/****************************************************************
 * Function declarations
 ***************************************************************/

/** 
 * @brief  Turns the given LED on or off
 * @param  led: The LED to switch
 * @param  action: Indicates if the LED should be turned on or off
 * @return TRUE on success else FALSE
 */
static BOOL Led_Switch(const LEDType led, BitAction const action);


/****************************************************************
 * User functions
 ***************************************************************/


/** 
 * Initializes all LEDs on the board
 */
void Led_Init(void)
{
  /* Enable clock for LED gpio port registers */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* LEDs are low active! */
  /* -> a '0' on the gpio pin turns the LED on */
  /* Thatswhy set all gpio pins to '1' before init */
  GPIOC->BSRR |= GPIO_Pin_0 | GPIO_Pin_1 |
                 GPIO_Pin_2 | GPIO_Pin_6 |
                 GPIO_Pin_7 | GPIO_Pin_8;
  
  /* Init all LEDs */
  GPIO_InitTypeDef gpioInit;
  GPIO_StructInit(&gpioInit);
  
  gpioInit.GPIO_Mode = GPIO_Mode_OUT;
  gpioInit.GPIO_OType = GPIO_OType_PP;
  gpioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                      GPIO_Pin_2 | GPIO_Pin_6 |
                      GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_Init(GPIOC, &gpioInit);
}

/**
 * Turns the given LED on by using module intern LES_Switch function
 */
BOOL Led_TurnOn(const LEDType led)
{
  /* Turn LED on */
  /* NOTE: LED is on if gpio is reset -> low active!!! */
  return Led_Switch(led, Bit_RESET);
}

/**
 * Turns the given LED off by using module intern LES_Switch function
 */
BOOL Led_TurnOff(const LEDType led)
{  
  /* Turn LED off */
  /* NOTE: LED is off if gpio is set -> low active!!! */
  return Led_Switch(led, Bit_SET);
}

/****************************************************************
 * Static functions
 ***************************************************************/

/**
 * Sets the given LED in the given state
 */
static BOOL Led_Switch(const LEDType led, BitAction const action)
{
  uint16_t GPIO_Pin = GPIO_Pin_0;
  
  /* Set gpio pin for the given LED */
  switch(led)
  {
    case LEDType_LED3:
      /* Gpio pin for LED3 is initial value! */
      break;
    
    case LEDType_LED4:
      GPIO_Pin = GPIO_Pin_1;
      break;
    
    case LEDType_LED5:
      GPIO_Pin = GPIO_Pin_2;
      break;
    
    case LEDType_LED_RGB_R:
      GPIO_Pin = GPIO_Pin_6;
      break;
    
    case LEDType_LED_RGB_G:
      GPIO_Pin = GPIO_Pin_7;
      break;
    
    case LEDType_LED_RGB_B:
      GPIO_Pin = GPIO_Pin_8;
      break;
    
    default:
      /* None LEDType value for LED given */
      return FALSE;
  }

  /* Write gpio pin */
  GPIO_WriteBit(GPIOC, GPIO_Pin, action);
  
  return TRUE;
}
