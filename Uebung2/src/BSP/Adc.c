/**
 ******************************************************************************
 * @file    Adc.h
 * @author  Julian Grasböck
 * @version V1.0.0
 * @date    05.12.2017
 * @brief   Abstracts Adc handling.
 * @copyright
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, 
 ******************************************************************************
 */

#include "Adc.h"
#include "stm32f0xx_adc.h"

/****************************************************************
 * User functions
 ***************************************************************/

/** 
 * Initializes the Adc in single conversion mode using the given channel
 */
int32_t Adc_Init(uint8_t const channel)
{
  ADC_InitTypeDef adcInitStr;
  
  /* Enable Adc peripheral clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* Initialize Adc configuration structure */
  adcInitStr.ADC_Resolution = ADC_Resolution_12b;
  adcInitStr.ADC_ContinuousConvMode = DISABLE;
  adcInitStr.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  adcInitStr.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO;
  adcInitStr.ADC_DataAlign = ADC_DataAlign_Right;
  adcInitStr.ADC_ScanDirection = ADC_ScanDirection_Upward;
  
  /* Initialize the Adc and the given channel */
  ADC_Init(ADC1, &adcInitStr);

  /* Enable Adc */
  ADC_Cmd(ADC1, ENABLE);
	
	return Adc_SetChannel(channel);
}

/** 
 * Function that triggers a conversion and returns the result
 */
int32_t Adc_GetValue(uint32_t const timeout)
{
	int32_t const adcResolution = 12;
	int32_t const adcReferenceVoltage = 3300;	
  int32_t ret = 0;
  uint32_t cnt = 0;
  
  /* Check if ADC is currently busy */
  if (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADSTART) == SET)
  {
    return ADC_RETURN_BUSY;
  }
  
  /* Trigger conversion */
  ADC_StartOfConversion(ADC1);

  /* Wait until conversion is finished or timeout appears*/
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET)
  {
		cnt++;
		
    /* Check if timout is reached */
    if((timeout != 0) &&
       (timeout == cnt))
    {
      return ADC_RETURN_TIMEOUT;
    }
  }
  
  /* Get ADC value */
  ret = (int32_t)(ADC_GetConversionValue(ADC1));
  
  /* convert to milli voltage and return result */
  return (ret * adcReferenceVoltage) / ((1 << adcResolution) - 1);
}

/** 
 * Function to set the Adc hardware channel that should be used
 */
int32_t Adc_SetChannel(uint8_t const channel)
{
	uint8_t const numOfHardwareChannels = 19;
		
  /* Check if vaild channel given */
  if(channel > numOfHardwareChannels)
  {
    return ADC_RETURN_BAD_CHANNEL;
  }
  
  /* Set the conversion channel */
	ADC1->CHSELR = (uint32_t)(1 << channel);
  
  return ADC_RETURN_OK;
}
