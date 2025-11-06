/**
  ******************************************************************************
  * @file    TaskWatch.c 
  * @author  Josef Langer
  * @version V1.1
  * @date    07.10.2022
  * @brief   Task Watch, prints the operating time on the display (hh:mm:ss)
  ******************************************************************************
  */
	

#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskWatch.h"

#include <stdio.h>

#define S_TO_MS     (uint32_t)1000
#define MIN_TO_S    (uint32_t)60
#define MIN_TO_MS   (uint32_t)60000
#define H_TO_MIN    (uint32_t)60
#define TFT_BUF_SZ  32 // buffersize tft message


void TaskWatch (const uint32_t sysTick)
{
    // get seconds
    uint32_t const seconds = (sysTick / S_TO_MS) % MIN_TO_S;
    uint32_t const minutes = (sysTick / MIN_TO_MS) % H_TO_MIN;
    
    // draw runtime
    char buf[32];   // buffer for text
    snprintf(buf, sizeof(buf), "runtime: %02d:%02d", minutes, seconds);
	Tft_DrawString(10, 18+3*24, buf);	
}
