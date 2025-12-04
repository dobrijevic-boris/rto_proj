
/**
 ******************************************************************************
 * @file    TftFont.h
 * @author  Julian Grasböck
 * @version V1.0.0
 * @date    22.11.2017
 * @brief   Definition of a Font
 * @copyright
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, 
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */
 
/** \defgroup TftFont
 *	\brief Definition of a Font
 *  \ingroup TftDisplay 
 *  @{
 */
 
#if!defined TFTFONT_H
#define TFTFONT_H

#include "StdDef.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/**
 * @brief Represents a font
 */
typedef struct
{
  uint16_t width;         /**< Character width */
  uint16_t height;        /**< Character height */
  uint32_t offset;        /**< Character offset */
  uint32_t count;         /**< Character count */
  uint8_t const * bitmap; /**< Characters bitmaps */
} TftFontType;

#endif

/** @}*/
