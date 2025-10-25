/**
  ******************************************************************************
  * @file    TaskMandelbrot.c 
  * @author  Josef Langer
  * @version V1.1
  * @date    07.10.2022
  * @brief   Task Mandelbrot, prints the Mandelbrot-Function on the display
  ******************************************************************************
  */
#include "BSP/Debug.h"
#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskMandelbrot.h"

// Quelle Algorithmus Mandelbrot: http://warp.povusers.org/Mandelbrot/ 

#define ImageHeight 150
#define ImageWidth  150
#define OFFSET		150
#define X_LOOP_CNT  5


static BOOL MandelBrot (void)
{
	double MinRe = -2.0;
	double MaxRe = 1.0;
	double MinIm = -1.2;
	double MaxIm = MinIm+(MaxRe-MinRe)*ImageHeight/ImageWidth;
	double Re_factor = (MaxRe-MinRe)/(ImageWidth-1);
	double Im_factor = (MaxIm-MinIm)/(ImageHeight-1);
	unsigned MaxIterations = 30;
    
	// make static to not loose progress
	static unsigned int x = 0;
	static unsigned int y = 0;
	
    unsigned int pixelsDrawn = 0;  // allways reset on re-entrancy
	for( ; y<ImageHeight; ++y)
	{		
		double c_im = MaxIm - y*Im_factor;
		for( ; x<ImageWidth; ++x)
		{
			double c_re = MinRe + x*Re_factor;
			double Z_re = c_re, Z_im = c_im;
			unsigned char isInside = TRUE;

			for(unsigned n=0; n<MaxIterations; ++n)
			{
				double Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
				if(Z_re2 + Z_im2 > 4)
				{
					isInside = FALSE;
					break;
				}
				Z_im = 2*Z_re*Z_im + c_im;
				Z_re = Z_re2 - Z_im2 + c_re;
			}

			
			if(isInside)
			{
				Tft_DrawPixel(y, x + OFFSET);	
			}
			
			if(++pixelsDrawn >= X_LOOP_CNT){		// cooperative yield
                ++x;
				return FALSE;
			}
		}
		
		if(x >= ImageWidth){	// reset outside of loop
			x = 0;
		}
	}
	
	if(y >= ImageHeight){	// reset outside of loop
		x = 0;
        y = 0;
	}
    
    return TRUE;
}


BOOL TaskMandelbrot (void)
{
	Tft_DrawString(10, 18+5*24, "MandelBrot ");
	return MandelBrot();	
}
