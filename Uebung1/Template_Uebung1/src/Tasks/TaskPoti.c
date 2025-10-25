/**
  ******************************************************************************
  * @file    TaskPoti.c 
  * @author  Josef Langer
  * @version V1.1
  * @date    07.10.2022
  * @brief   Task Poti, prints the value of the potentiometer on the display
  ******************************************************************************
  */
	

#include "BSP/TftDisplay.h"
#include "BSP/Adc.h"
#include "StdDef.h"
#include "TaskPoti.h"
#include <stdio.h>

#define TFT_BUF_SZ 20 // buffersize for tft message

void TaskPoti (void)
{
    int32_t potiVal = Adc_GetValue(1000);
    
    // draw potentiometer value
    char buf[TFT_BUF_SZ];
    snprintf(buf, sizeof(buf), "Poti: %d mV", potiVal);
	Tft_DrawString(10, 18+4*24, buf);	
}
