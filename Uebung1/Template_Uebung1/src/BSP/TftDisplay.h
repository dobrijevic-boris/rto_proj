/**
 ******************************************************************************
 * @file    TftDisplay.h
 * @author  Julian Grasböck
 * @version V1.0.0
 * @date    29.03.2017
 * @brief   TFT Display Controller Module (ST7789S controller chip)
 * @copyright
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, 
 * You may not use or copy this file except with written permission of the owners.
 ******************************************************************************
 */

/** \defgroup TftDisplay
 *	\brief Tft Display constants, types and usage functions
 *  @{
 */

#ifndef TFTDISPLAY_H
#define TFTDISPLAY_H

#include "StdDef.h"
#include "Fonts/TftFont.h"

/****************************************************************
 * Macros
 ***************************************************************/

/**
 * @brief If this define is set the display is used in landscape format.
 *        If not the display is used in portrait format */
//#define TFT_ORIENTATION_LANDSCAPE

/* Defines for the width and height depending on the orientation */
#ifdef TFT_ORIENTATION_LANDSCAPE
	#define TFT_WIDTH		320u
	#define TFT_HEIGHT	240u
#else
  #define TFT_WIDTH		240u
	#define TFT_HEIGHT	320u
#endif


/* Defines for some rgb16 colours
 * -> Color coding: 
 *    15..11 = R7..3 (Red)
 *    10..5  = G7..2 (Green)
 *     4..0  = B7..3 (Blue)
 */
#define TFT_COLOR_BLACK       ((uint16_t)0x0000)
#define TFT_COLOR_NAVY        ((uint16_t)0x000F)
#define TFT_COLOR_DARK_GREEN  ((uint16_t)0x03E0)
#define TFT_COLOR_DARK_CYAN   ((uint16_t)0x03EF)
#define TFT_COLOR_MAROON      ((uint16_t)0x7800)
#define TFT_COLOR_PURPLE      ((uint16_t)0x780F)
#define TFT_COLOR_OLIVE       ((uint16_t)0x7BE0)
#define TFT_COLOR_LIGHT_GREY  ((uint16_t)0xC618)
#define TFT_COLOR_DARK_GREY   ((uint16_t)0x7BEF)
#define TFT_COLOR_BLUE        ((uint16_t)0x001F)
#define TFT_COLOR_GREEN       ((uint16_t)0x07E0)
#define TFT_COLOR_CYAN        ((uint16_t)0x07FF)
#define TFT_COLOR_RED         ((uint16_t)0xF800)
#define TFT_COLOR_MAGENTA     ((uint16_t)0xF81F)
#define TFT_COLOR_YELLOW      ((uint16_t)0xFFE0)
#define TFT_COLOR_WHITE       ((uint16_t)0xFFFF)

/****************************************************************
 * User functions
 ***************************************************************/

/** 
 * @brief Initializes the TftDisplay Module
 */
void Tft_Init(void);

/**
 * @brief Set foreground colour
 * @param colour: Colour value in rgb 16 bit format
 *
 * Colour coding:
 *		15..11 = Red
 *		10..5  = Green
 *		 4..0  = Blue
 */
void Tft_SetForegroundColourRgb16(uint16_t const colour);

/**
 * @brief Set background colour
 * @param colour: Colour value in rgb 16 bit format
 */
void Tft_SetBackgroundColourRgb16(uint16_t const colour);

/**
 * @brief Clear screen (with active background colour)
 * @retval TRUE if the function succeeded else FALSE
 */
void Tft_ClearScreen(void);

/**
 * @brief Set active font
 * @param font: Pointer to font structure
 * @retval TRUE if the function succeeded else FALSE
 */
BOOL Tft_SetFont(TftFontType const * const font);

/**
 * @brief Draw pixel (in active foreground colour)
 * @param x: X coordinate of the pixel
 * @param y: Y coordinate of the pixel
 * @retval TRUE if the function succeeded else FALSE
 */
BOOL Tft_DrawPixel(uint16_t const x, uint16_t const y);

/**
 * @brief Draw horizontal line (in active foreground colour)
 * @param x: X coordinate from where to start drawing
 * @param y: Y coordinate from where to start drawing
 * @param length: Line length in pixels
 * @retval TRUE if the function succeeded else FALSE
 */
BOOL Tft_DrawHLine(uint16_t const x, uint16_t const y, uint16_t const length);

/**
 * @brief Draw vertical line (in active foreground colour)
 * @param x: X coordinate from where to start drawing
 * @param y: Y coordinate from where to start drawing
 * @param length: Line length in pixels
 * @retval TRUE if the function succeeded else FALSE
 */
BOOL Tft_DrawVLine(uint16_t const x, uint16_t const y, uint16_t const length);

/**
 * @brief Draw character (in active foreground colour)
 * @param x: X coordinate refering to the left edge of the char
 * @param y: Y coordinate refering to the upper edge of the char
 * @param ch Character
 * @retval TRUE if the function succeeded else FALSE
 */
BOOL Tft_DrawChar(uint16_t const x, uint16_t const y, char ch);

/**
 * @brief Draw string (in active foreground colour)
 * @param pos Position where to start the line
 *						(x = 0 left corner; y = 0 upper corner)
 * @param str Null-terminated String
 * @retval TRUE if the function succeeded else FALSE
 */
BOOL Tft_DrawString(uint16_t x, uint16_t const y, char const * str);

#endif

/** @}*/
