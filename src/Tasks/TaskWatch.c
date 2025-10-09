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


void TaskWatch (void)
{
	Tft_DrawString(10, 18+3*24, "Watch ");	
}
