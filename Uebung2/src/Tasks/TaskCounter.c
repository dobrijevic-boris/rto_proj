/**
  ******************************************************************************
  * @file    TaskCounter.c 
  * @author  Josef Langer
  * @version V1.1
  * @date    07.10.2022
  * @brief   Task Counter, Increases the counter and prints it on the display
  ******************************************************************************
  */
	
#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskCounter.h"
#include <stdio.h>

#define MAX_LEN 14		// Maximale Anzahl Zeichen pro Zeile
static char tmpBuf[MAX_LEN];

void TaskCounter (void)
{
	static uint32_t counter = 0;
	
	counter++;
	snprintf(tmpBuf, MAX_LEN, "%d", counter);
	Tft_DrawString(10, 18+0*24, "Cnt ");	
	Tft_DrawString(10 + 16*7, 18+0*24, tmpBuf);		
}
