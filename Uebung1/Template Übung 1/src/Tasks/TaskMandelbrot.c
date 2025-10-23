/**
  ******************************************************************************
  * @file    TaskMandelbrot.c 
  * @author  Josef Langer
  * @version V1.1
  * @date    07.10.2022
  * @brief   Task Mandelbrot, prints the Mandelbrot-Function on the display
  ******************************************************************************
  */
	

#include "BSP/TftDisplay.h"
#include "StdDef.h"
#include "TaskMandelbrot.h"

// Quelle Algorithmus Mandelbrot: http://warp.povusers.org/Mandelbrot/ 

#define ImageHeight 150
#define ImageWidth  150
#define OFFSET			150


static void MandelBrot (void)
{
	double MinRe = -2.0;
	double MaxRe = 1.0;
	double MinIm = -1.2;
	double MaxIm = MinIm+(MaxRe-MinRe)*ImageHeight/ImageWidth;
	double Re_factor = (MaxRe-MinRe)/(ImageWidth-1);
	double Im_factor = (MaxIm-MinIm)/(ImageHeight-1);
	unsigned MaxIterations = 30;

	for(unsigned y=0; y<ImageHeight; ++y)
	{
			double c_im = MaxIm - y*Im_factor;
			for(unsigned x=0; x<ImageWidth; ++x)
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
					if(isInside) { Tft_DrawPixel(y, x + OFFSET); }
			}
	}
}


void TaskMandelbrot (void)
{
	Tft_DrawString(10, 18+5*24, "ManBr ");
	MandelBrot();	
}
