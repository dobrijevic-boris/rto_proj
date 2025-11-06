/**
  ******************************************************************************
  * @file    TaskLed.c 
  * @author  Josef Langer
  * @version V1.1
  * @date    07.10.2022
  * @brief   Task LED, Running light, changes every 100ms between LED 3,4 and 5
  ******************************************************************************
  */
	
#include "StdDef.h"
#include "TaskLed.h"
#include "BSP/Led.h"

// Define LED task states
typedef enum {
    LED_STATE_START,
    LED_STATE_LED3_ON,
    LED_STATE_LED4_ON,
    LED_STATE_LED5_ON
} LED_STATE_t;

void TaskLed (void)
{
 
    static LED_STATE_t state = LED_STATE_START;

    // Simple state machine
    switch (state)
    {
        case LED_STATE_LED3_ON:
            Led_TurnOn(LEDType_LED3);
            Led_TurnOff(LEDType_LED4);
            Led_TurnOff(LEDType_LED5);
            state = LED_STATE_LED4_ON;
            break;

        case LED_STATE_LED4_ON:
            Led_TurnOff(LEDType_LED3);
            Led_TurnOn(LEDType_LED4);
            state = LED_STATE_LED5_ON;
            break;

        case LED_STATE_LED5_ON:
            Led_TurnOff(LEDType_LED4);
            Led_TurnOn(LEDType_LED5);
            state = LED_STATE_LED3_ON;
            break;
        case LED_STATE_START: // fall through to default
        default:
            Led_TurnOff(LEDType_LED3);
            Led_TurnOff(LEDType_LED4);
            Led_TurnOff(LEDType_LED5);
            state = LED_STATE_LED3_ON;
            break;
    }
}

