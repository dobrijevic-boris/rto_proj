/**
 ******************************************************************************
 * @file    TftDisplay.c
 * @author  Julian Grasböck
 * @version V1.0.0
 * @date    15.11.2017
 * @brief   TFT Display Controller Module (ST7789S controller chip)
 * @copyright
 * Copyright (c) FH OÖ Forschungs und Entwicklungs GmbH, 
 ******************************************************************************
 */

#include "TftDisplay.h"
#include "stm32f0xx_gpio.h"
#include <assert.h>

/****************************************************************
 * Macros
 ***************************************************************/

/****************************************************************
 * Type definitions
 ***************************************************************/

/*
 * Defines the commands for the display
 * See TSC2046E ref. manual
 */
typedef enum
{
	Tft_Cmd_SLPOUT = 0x11,    /* Sleep out (It will be necessary to wait */
	Tft_Cmd_DISPOFF = 0x28,   /* Display off */
	Tft_Cmd_DISPON = 0x29,    /* Display on */
	Tft_Cmd_CASET = 0x2A,     /* Column adress set */
	Tft_Cmd_RASET = 0x2B,     /* Row adress set */
	Tft_Cmd_RAMWR = 0x2C,     /* Memory write */
	Tft_Cmd_MADCTL = 0x36,    /* Memory data access control */
	Tft_Cmd_COLMOD = 0x3A     /* Interface pixel format */
} Tft_CmdType;

/**
 * @brief Enum that defines the positions of the colours in the colour array
 */
typedef enum
{
	Tft_ColourIdx_Background = 0, /**< Index of the background colour */
	Tft_ColourIdx_Foreground,     /**< Index of the foreground colour */
	Tft_ColourIdx_Count						/**< Num of colours */
} Tft_ColourIdx;

/**
 * @brief Structure to initialize the module
 */
typedef struct
{
	uint16_t colours[Tft_ColourIdx_Count];	/**< Background and Foreground Colours */
	TftFontType const * font;								/**< Font to be used when writing chars */
} Tft_ConfigType;

/****************************************************************
 * Constants
 ***************************************************************/

/* Constants for the pin placement */
/* Current setup -> APUS V1.2 */
static uint16_t const Tft_DB = GPIO_Pin_0 | GPIO_Pin_1 |
    GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
static uint16_t const Tft_nCS = GPIO_Pin_8;
static uint16_t const Tft_nWR = GPIO_Pin_10;
static uint16_t const Tft_nRD = GPIO_Pin_11;
static uint16_t const Tft_DC = GPIO_Pin_9;
static uint16_t const Tft_nRES = GPIO_Pin_10;

/* Interface port */
static GPIO_TypeDef * const Tft_Port = GPIOE;
static GPIO_TypeDef * const Tft_ResPort = GPIOF;

/* Parameter values used to init controller */
#ifdef TFT_ORIENTATION_LANDSCAPE
  static uint8_t const Tft_OrientInitVal = 0xA0;
#else
  static uint8_t const Tft_OrientInitVal = 0x00;
#endif
static uint8_t const Tft_SelColMode16bpp = 0x05;

/****************************************************************
 * Function declarations
 ***************************************************************/

static void Tft_SetWindow(uint16_t const x, uint16_t const y, uint16_t const width,
                          uint16_t const height);
static void Tft_WriteCmd(Tft_CmdType const command);
static void Tft_WriteData(uint8_t const data);
static void Tft_WritePixel(uint16_t const rgbPixelValue);
static void Tft_WritePixels(uint16_t const rgbPixelValue, uint32_t const num);

static uint32_t Tft_ToBSRRValue(uint8_t data);

/****************************************************************
 * Global variables
 ***************************************************************/

static Tft_ConfigType Tft_Cfg = 
{
  .colours[Tft_ColourIdx_Background] = TFT_COLOR_WHITE,
  .colours[Tft_ColourIdx_Foreground] = TFT_COLOR_BLACK,
  .font = NULL_PTR
};

/****************************************************************
 * User functions
 ***************************************************************/

static void Tft_InitGPIOs(void)
{
  /* Enable clocks for gpio ports */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
  
  /* Init all pins except reset */
  GPIO_InitTypeDef gpioInit;
  GPIO_StructInit(&gpioInit);
  gpioInit.GPIO_Mode = GPIO_Mode_OUT;
  gpioInit.GPIO_OType = GPIO_OType_PP;
  gpioInit.GPIO_Pin = Tft_nCS | Tft_nWR | Tft_nRD | Tft_DC | Tft_DB;
  gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(Tft_Port, &gpioInit);
  
  /* Init reset pin */
  gpioInit.GPIO_Pin = Tft_nRES;
  GPIO_Init(Tft_ResPort, &gpioInit);
}

/*
 * Function that initializes the module and the display
 */
void Tft_Init(void)
{	
  /* Init all GPIOs */
  Tft_InitGPIOs();
  
  /* Set read and write inactive */
  Tft_Port->BSRR = Tft_nRD | Tft_nWR;
  
  /* Set chip enabled */
  /* Chip can always stay enabled cause there is no other chip */
  /* on the same bus */
  Tft_Port->BRR = Tft_nCS;
  
  /* Set reset inactive */
  Tft_ResPort->BSRR = Tft_nRES;
		
  /* TODO: wait for TRT time -> see (RefManual page 46) */
  
  /* After S/W reset, H/W reset and power on sequence display is in sleep mode */
  /* -> Exit sleep mode */
  Tft_WriteCmd(Tft_Cmd_SLPOUT);

  /* Set method of pixel data transfer */
  Tft_WriteCmd(Tft_Cmd_MADCTL);
  /* Init data transfer */
  Tft_Port->BSRR = Tft_DC;
  Tft_WriteData(Tft_OrientInitVal);

  /* Set choosen colour resolution */
  Tft_WriteCmd(Tft_Cmd_COLMOD);
  /* Init data transfer */
  Tft_Port->BSRR = Tft_DC;
  Tft_WriteData(Tft_SelColMode16bpp);
  
  /* Enable display */
  Tft_WriteCmd(Tft_Cmd_DISPON);
}

/*
 * Sets the given colour as the current foreground colour
 */
void Tft_SetForegroundColourRgb16(uint16_t const colour)
{
	Tft_Cfg.colours[Tft_ColourIdx_Foreground] = colour;
}

/*
 * Sets the given colour as the current background colour
 */
void Tft_SetBackgroundColourRgb16(uint16_t const colour)
{
	Tft_Cfg.colours[Tft_ColourIdx_Background] = colour;
}

/*
 * Sets the whole display area to the background colour
 */
void Tft_ClearScreen(void)
{
  /* Set drawing region and draw in the given colour */
	Tft_SetWindow(0, 0, TFT_WIDTH, TFT_HEIGHT);
	/* Draw the arrea with the given colour */
	Tft_WritePixels(Tft_Cfg.colours[Tft_ColourIdx_Background],
                  TFT_WIDTH * TFT_HEIGHT);
}

/*
 * Sets the given font as active font
 */
BOOL Tft_SetFont(TftFontType const * const font)
{
	/* Check given font */
	if((font != NULL_PTR) &&
		(font->bitmap != NULL_PTR) &&
		(font->height <= TFT_HEIGHT) &&
		(font->width <= TFT_WIDTH) &&
		(font->height != 0) &&
		(font->width != 0))
	{
		Tft_Cfg.font = font;
		return TRUE;
	}
	return FALSE;
}

/*
 * Draws one pixel at the given position
 */
BOOL Tft_DrawPixel(uint16_t const x, uint16_t const y)
{
	if((x < TFT_WIDTH) &&
		 (y < TFT_HEIGHT))
	{
		/* Set drawing region to pixel location */
		Tft_SetWindow(x, y, 1, 1);
		/* Send write command */
		Tft_WriteCmd(Tft_Cmd_RAMWR);
		/* Init pixel transfer */
		Tft_Port->BSRR = Tft_DC;
		/* Send pixel */
		Tft_WritePixel(Tft_Cfg.colours[Tft_ColourIdx_Foreground]);
		
		return TRUE;
	}
	return FALSE;
}

/*
 * Draws a horizontal line at the given position with the given length
 */
BOOL Tft_DrawHLine(uint16_t const x, uint16_t const y, uint16_t const length)
{
	if(((x + length) <= TFT_WIDTH) &&
		(y < TFT_HEIGHT))
	{
		if(length > 0)
		{
      /* Set drawing region and draw in the given colour */
      Tft_SetWindow(x, y, length, 1);
      /* Draw the arrea with the given colour */
      Tft_WritePixels(Tft_Cfg.colours[Tft_ColourIdx_Foreground],
                      length);
		}
		/* else Nothing to do */
		return TRUE;
	}
	return FALSE;
}

/*
 * Draws a vertical line at the given position with the given length
 */
BOOL Tft_DrawVLine(uint16_t const x, uint16_t const y, uint16_t const length)
{
	if(((y + length) <= TFT_HEIGHT) &&
		(x < TFT_WIDTH))
	{
		if(length > 0)
		{
            /* Set drawing region and draw in the given colour */
      Tft_SetWindow(x, y, 1, length);
      /* Draw the arrea with the given colour */
      Tft_WritePixels(Tft_Cfg.colours[Tft_ColourIdx_Foreground],
                      length);
		}
		/* else Nothing to do */
		return TRUE;
	}
	return FALSE;
}


/*
 * Draws the given char at the given position
 */
BOOL Tft_DrawChar(uint16_t const x, uint16_t const y, char ch)
{	
	/* Ptr to the font */
	TftFontType const * const font = Tft_Cfg.font;
	
	/* Check user given font */
	if(font == NULL_PTR)
	{
		return FALSE;
	}
	
	/* Check if font contains char */
	if ((ch < font->offset) ||
			(ch > (font->offset + font->count)))
	{
		return FALSE;
	}
	
	/* Subtract bitmap character offset from actual ASCII character value */
  ch -= font->offset;
  
  /* Calculate the number of bytes that are used for one row */
  uint32_t bytesForOneRow = font->width / 8;
  /* Check if division results in rest != 0 */
  /* If so one more Byte is used for one row */
  if((font->width % 8) != 0)
  {
    bytesForOneRow++;
  }

	/* Represents the total number of bytes used for one character */
	uint32_t const bytesForOneChar = bytesForOneRow * font->height;
	/* Get the pointer where to start in the bitmap */
  uint8_t const * currByte = font->bitmap + (((uint32_t)ch) * bytesForOneChar);

	/* Set drawing window */
  Tft_SetWindow(x, y, font->width, font->height);
	/* Send write command */
	Tft_WriteCmd(Tft_Cmd_RAMWR);
	/* Init pixel transfer */
	Tft_Port->BSRR = Tft_DC;
  
  /* Draw row after row */
  for(uint16_t row = 0; row < font->height; row++)
  {
    /* Go through all pixels in the row */
    for(uint16_t col = 0; col < font->width; col++)
    {
      /* Check if current bit is set */
      if((*currByte) & (1 << (col % 8)))
      {
        /* If bit is set draw pixel */
        Tft_WritePixel(Tft_Cfg.colours[Tft_ColourIdx_Foreground]);
      }
      else
      {
        /* Else draw pixel in bakground colour */
        Tft_WritePixel(Tft_Cfg.colours[Tft_ColourIdx_Background]);
      }
      
      /* Check if next bit is in next byte */
      if(((col & 7) == 7) &&
      /* Check if pixel left in row */
          (col < (font->width - 1)))
      {
        currByte++;
      }
    }
    /* Inc pointer after each row */
    currByte++;
  }
  
	return TRUE;
}

/*
 * Prints the given string in foreground colour
 */
BOOL Tft_DrawString(uint16_t x, uint16_t const y, char const * str)
{
	static char const strEnd = '\0';
	
	/* Check user config and given string ptr */
	if((Tft_Cfg.font == NULL_PTR) ||
		(str == NULL_PTR))
	{
		return FALSE;
	}

	TftFontType const * const font = Tft_Cfg.font;
	
	/* Draw char after char till terminating 0 is found */
	while((*str) != strEnd)
	{
		/* Draw current char */
		if(Tft_DrawChar(x, y, *str) == FALSE)
		{
			return FALSE;
		}
    /* Increment x value */
    x += font->width;
    /* Select next char */
    str++;
	}
	return TRUE;
}

/****************************************************************
 * Static functions
 ***************************************************************/

/*
 * Sets the drawing region of the display
 */
static void Tft_SetWindow(uint16_t const x, uint16_t const y, uint16_t const width,
                          uint16_t const height)
{
	assert(width > 0);
	assert(height > 0);
	assert((x + width) <= TFT_WIDTH);
	assert((y + height) <= TFT_HEIGHT);
	
  uint16_t const xe = x + width - 1;
	uint16_t const ye = y + height - 1;

	/* Set column span */
	Tft_WriteCmd(Tft_Cmd_CASET);
	/* Init data transfer */
	Tft_Port->BSRR = Tft_DC;
	Tft_WriteData((uint8_t)(x >> 8u));	/* Column address start MSB */
	Tft_WriteData((uint8_t)x);          /* Column address start LSB */
	Tft_WriteData((uint8_t)(xe >> 8u)); /* Column address end MSB */
	Tft_WriteData((uint8_t)xe);         /* Column address end LSB */
	
	/* Set row span */
	Tft_WriteCmd(Tft_Cmd_RASET);
	/* Init data transfer */
	Tft_Port->BSRR = Tft_DC;
	Tft_WriteData((uint8_t)(y >> 8u));	/* Row address start MSB */
	Tft_WriteData((uint8_t)y);          /* Row address start LSB */
	Tft_WriteData((uint8_t)(ye >> 8u)); /* Row address end MSB */
	Tft_WriteData((uint8_t)ye);         /* Row address end LSB */
}

/*
 * Function to write a user command to the display
 */
static void Tft_WriteCmd(Tft_CmdType const command)
{
	/* Init command transfer */
	Tft_Port->BSRR = Tft_DC << 16u;
	/* Send command */
	Tft_WriteData((uint8_t)command);
}

/*
 * Function to write data to the display
 */
static void Tft_WriteData(uint8_t const data)
{
	/* Initiate write cycle and put data on GPIOs */
	Tft_Port->BSRR = Tft_ToBSRRValue(data) | (Tft_nWR << 16u);
	/* Stop writing sequence */
  Tft_Port->BSRR = Tft_nWR;
}

/*
 * Function to write a single pixel
 */
static void Tft_WritePixel(uint16_t const rgbPixelValue)
{
	/* Write first byte */
  Tft_WriteData((uint8_t)(rgbPixelValue >> 8u));
	/* Write second byte */
  Tft_WriteData((uint8_t)(rgbPixelValue));
}

/*
 * Function to write num pixels in the given colour
 */
static void Tft_WritePixels(uint16_t const rgbPixelValue, uint32_t num)
{
	/* Send write command */
	Tft_WriteCmd(Tft_Cmd_RAMWR);
	/* Init pixel transfer */
	Tft_Port->BSRR = Tft_DC;
	
	/* Seperate transmit bytes and get the corresponding bsrr value */
	uint32_t bsrrVal1 = Tft_ToBSRRValue((uint8_t)(rgbPixelValue >> 8u)) |
		(Tft_nWR << 16u);
	uint32_t bsrrVal2 = Tft_ToBSRRValue((uint8_t)rgbPixelValue) |
		(Tft_nWR << 16u);
	
	while((num--) > 0)
	{
		/* Initiate write cycle and put data on GPIOs */
		Tft_Port->BSRR = bsrrVal1;
		/* Stop writing sequence */
		Tft_Port->BSRR = Tft_nWR;
		/* Initiate write cycle and put data on GPIOs */
		Tft_Port->BSRR = bsrrVal2;
		/* Stop writing sequence */
		Tft_Port->BSRR = Tft_nWR;
	}
}

/*
 * Function that returns the 32 bit value that sets the given data
 * on the DB pins when written to the BSRR register
 */
static uint32_t Tft_ToBSRRValue(uint8_t data)
{
	/* BSRR => Reset-> P15 | P14 |...| P7 | P6 | P5 | P4 | P3 | P2 | P1 | P0
	 *				 Set->	 P15 | P14 |...| P7 | P6 | P5 | P4 | P3 | P2 | P1 | P0
	 * Therefor bits to be set always have to be in first byte position of lower
	 * halfword and bits to be cleared have to be in first byte position of upper
	 * halfword
	 *
	 * Add '1' values of data to BSRR write value */
	uint32_t bsrrVal = data;
	/* Invert data to get '0' values */
	data = ~data;
	
	/* Add '0' values of data to BSRR write value and return result */	
	return bsrrVal | (data << 16u);
}
