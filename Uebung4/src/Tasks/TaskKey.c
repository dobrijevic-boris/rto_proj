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
#include "BSP/Debug.h"
#include "BSP/Key.h"
#include "BSP/Led.h"
#include "StdDef.h"
#include "TaskKey.h"
#include "StdDef.h"
#include <stdio.h>
#include "APOS.h"
#include "TaskAll.h"

void TaskKey (void)
{
    while(1) {
        Debug_SwitchDebugPin(DEBUG_PIN_TASKKEY, Bit_SET);	
        BOOL keyState_User0 = Key_GetState(KeyType_USER0);
        BOOL keyState_User1 = Key_GetState(KeyType_USER1);
        BOOL keyState_WakeUp = Key_GetState(KeyType_WAKEUP);
        
        // change color when key pressed
        if(keyState_User0) {
            Tft_SetForegroundColourRgb16(TFT_COLOR_PURPLE);
            APOS_SignalEvent(&TCB_Tasks[TASK_LED], EVENT_LED_WKUP);
        }
        else if(keyState_User1) {
            Tft_SetForegroundColourRgb16(TFT_COLOR_DARK_GREEN);
            APOS_SignalEvent(&TCB_Tasks[TASK_LED], EVENT_LED_WKUP);
        } else if(keyState_WakeUp) {
            Tft_SetForegroundColourRgb16(TFT_COLOR_RED);
            APOS_SignalEvent(&TCB_Tasks[TASK_LED], EVENT_LED_WKUP);
        }
        // draw key state
        char buf[32];   // buffer for text
        snprintf(buf, sizeof(buf), "User0:%d", keyState_User0);
        
        APOS_MUTEX_LockBlocked(&mutexTft);
        Tft_DrawString(10, 18 + 1 * 24, buf);
        APOS_MUTEX_Unlock(&mutexTft);
        
        snprintf(buf, sizeof(buf), "User1:%d", keyState_User1);
        
        APOS_MUTEX_LockBlocked(&mutexTft);
        Tft_DrawString(60, 18 + 1 * 24, buf);
        APOS_MUTEX_Unlock(&mutexTft);
        
        snprintf(buf, sizeof(buf), "Wakeup:%d", keyState_WakeUp);
        APOS_MUTEX_LockBlocked(&mutexTft);        
        Tft_DrawString(120, 18 + 1 * 24, buf);
        APOS_MUTEX_Unlock(&mutexTft);
        
        
        Debug_SwitchDebugPin(DEBUG_PIN_TASKKEY, Bit_RESET);
        APOS_TaskDelay(100);
    }
  
}
