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
#include "BSP/Debug.h"
#include "BSP/Adc.h"
#include "StdDef.h"
#include "TaskPoti.h"
#include <stdio.h>
#include "APOS.h"
#include "TaskAll.h"

#define TFT_BUF_SZ 20 // buffersize for tft message

void TaskPoti (void)
{
    while(1) {
        
        Debug_SwitchDebugPin(DEBUG_PIN_TASKPOTI, Bit_SET);
        int32_t potiVal = Adc_GetValue(1000);
        
        // draw potentiometer value
        char buf[TFT_BUF_SZ];
        snprintf(buf, sizeof(buf), "Poti: %d mV  ", potiVal);
        
        APOS_MUTEX_LockBlocked(&mutexTft);
        Tft_DrawString(10, 18+4*24, buf);
        APOS_MUTEX_Unlock(&mutexTft);
        
        Debug_SwitchDebugPin(DEBUG_PIN_TASKPOTI, Bit_RESET);
        APOS_TaskDelay(100);
    }
}
