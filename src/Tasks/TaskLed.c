/**
  ******************************************************************************
  * @file    TaskLed.c 
  * @author  Josef Langer
  * @version V1.1
  * @date    07.10.2022
  * @brief   Task LED, Running light, changes every 100ms between LED 3,4 and 5
  ******************************************************************************
  */
	
#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskLed.h"


void TaskLed (void)
{
	Tft_DrawString(10, 18+2*24, "Led ");	
}
