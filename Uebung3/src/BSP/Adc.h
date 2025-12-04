/**
 ******************************************************************************
 * @file    Adc.h
 * @author  Julian Grasböck
 * @version V1.0.0
 * @date    05.12.2017
 * @brief   Abstracts Adc handling.
 * @copyright
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, 
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

/** \defgroup Adc
 *	\brief Adc constants, types and usage functions
 *  @{
 */

#ifndef ADC_H
#define ADC_H

#include "StdDef.h"

/****************************************************************
 * Macros
 ***************************************************************/

/* Return values */
#define ADC_RETURN_OK           ((int32_t)(0))
#define ADC_RETURN_BAD_CHANNEL  ((int32_t)(-1))
#define ADC_RETURN_BUSY         ((int32_t)(-2))
#define ADC_RETURN_TIMEOUT      ((int32_t)(-3))

/* ADC Channels */
#define ADC_CHANNEL_POTENTIOMETER	((uint8_t)(14))

/****************************************************************
 * User functions
 ***************************************************************/

/**
 * @brief Initializes the adc using 12 bit resolution.
 * @param channel The Adc hardware channel that should be used in conversions.
 * @return ADC_RETURN_OK if the given channel is valid else
 *         ADC_RETURN_BAD_CHANNEL
 */
int32_t Adc_Init(uint8_t const channel);

/**
 * @brief Gets the adc value.
 * @param timeout Timeout to wait for the end of the conversion.
                  A value of 0 will cause infinite timeout.
 * @return The adc conversion value in mV or
 *         ADC_RETURN_BUSY if the Adc is currently working or
 *         ADC_RETURN_TIMEOUT if the conversion could not be completed
 *         within the given timeout.
 */
int32_t Adc_GetValue(uint32_t const timeout);

/**
 * @brief Selects the Adc hardware channel to use for conversion.
 * @param channel The Adc hardware channel that should be used in conversions.
 * @return ADC_RETURN_OK if the given channel is valid else
 *         ADC_RETURN_BAD_CHANNEL
 */
int32_t Adc_SetChannel(uint8_t const channel);

#endif
