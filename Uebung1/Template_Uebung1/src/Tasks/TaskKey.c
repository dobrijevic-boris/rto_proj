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
#include "BSP/Key.h"
#include "StdDef.h"
#include "TaskKey.h"
#include "StdDef.h"
#include <stdio.h>

void TaskKey (void)
{
    BOOL keyState_User0 = Key_GetState(KeyType_USER0);
    BOOL keyState_User1 = Key_GetState(KeyType_USER1);
    BOOL keyState_WakeUp = Key_GetState(KeyType_WAKEUP);
    
    // change color when key pressed
    if(keyState_User0) {
        Tft_SetForegroundColourRgb16(TFT_COLOR_BLUE);
    }
    else if(keyState_User1) {
        Tft_SetForegroundColourRgb16(TFT_COLOR_GREEN);
    } else if(keyState_WakeUp) {
        Tft_SetForegroundColourRgb16(TFT_COLOR_BLACK);
    }
    char buf[32];   // buffer for text
    snprintf(buf, sizeof(buf), "User0: %d", keyState_User0);
    Tft_DrawString(10, 18 + 1 * 24, buf);
  
  
  
  
  
}
