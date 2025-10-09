/**
  ******************************************************************************
  * @file    TaskKey.c 
  * @author  Josef Langer
  * @version V1.1
  * @date    07.10.2022
  * @brief   Task Key, Reads the Key values and prints it on the display
	*					 Key USER changes the colour blue->green->black->blue->...
  ******************************************************************************
  */
	
#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskKey.h"


void TaskKey (void)
{
	Tft_DrawString(10, 18+1*24, "Key ");	
}
