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
#include "StdDef.h"
#include "TaskPoti.h"


void TaskPoti (void)
{
	Tft_DrawString(10, 18+4*24, "Poti ");	
}
