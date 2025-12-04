/**
 ******************************************************************************
 * @file    Led.h
 * @author  Julian Grasböck
 * @version V1.0.0
 * @date    14.11.2017
 * @brief   Abstracts LED handling.
 * @copyright
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, 
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

/** \defgroup Led
 *	\brief Led constants, types and usage functions
 *  @{
 */

#ifndef LED_H
#define LED_H

#include "StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/** @brief Enum type for all LEDs on the apus board */
typedef enum
{
  LEDType_LED3 = 0,
  LEDType_LED4,
  LEDType_LED5,
  LEDType_LED_RGB_R,
  LEDType_LED_RGB_G,
  LEDType_LED_RGB_B,
  LEDType_Count
} LEDType;

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * @brief Initializes the LED module
 */
void Led_Init(void);

/**
 * @brief  Turns the given LED on 
 * @param  led: LED to turn on
 * @return TRUE on success else FALSE
 */
BOOL Led_TurnOn(const LEDType led);

/**
 * @brief  Sets off-time of the LED to 100%
 * @param  led: LED to turn off
 * @return TRUE on success else FALSE
 */
BOOL Led_TurnOff(const LEDType led);

#endif
