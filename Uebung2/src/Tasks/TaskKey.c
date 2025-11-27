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
#include "StdDef.h"
#include "TaskKey.h"
#include "StdDef.h"
#include <stdio.h>
#include "APOS.h"
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
        }
        else if(keyState_User1) {
            Tft_SetForegroundColourRgb16(TFT_COLOR_DARK_GREEN);
        } else if(keyState_WakeUp) {
            Tft_SetForegroundColourRgb16(TFT_COLOR_RED);
        }
        // draw key state (split into three separate critical regions to limit max cs time
        char buf[32];   // buffer for text
        snprintf(buf, sizeof(buf), "User0:%d", keyState_User0);
        APOS_EnterCriticalRegion();
        Tft_DrawString(10, 18 + 1 * 24, buf);
        APOS_ExitCriticalRegion();
        
        APOS_EnterCriticalRegion();
        snprintf(buf, sizeof(buf), "User1:%d", keyState_User1);
        
        Tft_DrawString(60, 18 + 1 * 24, buf);
        APOS_ExitCriticalRegion();
        
        APOS_EnterCriticalRegion();
        snprintf(buf, sizeof(buf), "Wakeup:%d", keyState_WakeUp);
        Tft_DrawString(120, 18 + 1 * 24, buf);
        APOS_ExitCriticalRegion();
        
        
        Debug_SwitchDebugPin(DEBUG_PIN_TASKKEY, Bit_RESET);
        APOS_TaskDelay(20);
    }
  
}
