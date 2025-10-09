/**
 ******************************************************************************
 * @file    Key.c
 * @author  Julian Grasböck
 * @version V1.0.0
 * @date    14.11.2017
 * @brief   Abstracts key handling.
 * @copyright
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

/** \defgroup Key
 *	\brief Key usage functions
 *  @{
 */


#ifndef KEY_H
#define KEY_H

#include "StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/
 
/** @brief Enum type for all keys on the apus board */
typedef enum
{
  KeyType_WAKEUP = 0,
  KeyType_USER0,
  KeyType_USER1,
  KeyType_Count
} KeyType;

/****************************************************************
 * User functions
 ***************************************************************/

/** 
 * @brief Initializes the keys of the board
 */
void Key_Init(void);

/** 
 * @brief Returns the current state of the key
 * @param key: Key that should be read
 * @return TRUE if pressed, FALSE if released
 */
BOOL Key_GetState(KeyType const key);

#endif

/** @}*/
