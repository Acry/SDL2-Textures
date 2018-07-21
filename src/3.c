//BEGIN HEAD
//BEGIN DESCRIPTION

/* 3D noise
 * 
 */

/* DEFINED PROGRESS GOALS
 * 
 * None atm
 * 
 */
//END   DESCRIPTION

//BEGIN INCLUDES
//system headers
#include <math.h>
//local headers
#include "helper.h"
#include <stdlib.h>
//END   INCLUDES

//BEGIN CPP DEFINITIONS
#define WHITE 255,255,255,255
#define WW 512
#define WH 512

#define noiseWidth  512
#define noiseHeight 512
#define noiseDepth   64
//END   CPP DEFINITIONS

//BEGIN DATASTRUCTURES
double noise[noiseDepth][noiseHeight][noiseWidth]; // 3D Noise-Array
typedef struct HSL_Color
{
	Uint8 h;
	Uint8 s;
	Uint8 l;
	
} HSL_Color;
HSL_Color ColorHSL;
//END	DATASTRUCTURES

//BEGIN GLOBALS
int ww=WW;
int wh=WH;

SDL_Surface    *temp_surface	= NULL;
//BEGIN VISIBLES
//END 	VISIBLES

//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
double 	smoothNoise(double , double, double);
void 	generateNoise(void);
double 	turbulence(double, double, double, double);
float  	fTimeDelta		(void); // frame time
float 	fTime(void);			// runtime
SDL_Color HSLtoRGB(HSL_Color);
//END	FUNCTION PROTOTYPES

//END 	HEAD

//BEGIN MAIN FUNCTION
int main(int argc, char *argv[])
{

(void)argc;
(void)argv;

//BEGIN INIT
init();

//BEGIN WINDOW
SDL_SetWindowPosition(Window,0,0);
SDL_SetWindowSize(Window,ww,wh);
SDL_SetWindowTitle(Window, "3D Random Noise");
SDL_ShowWindow(Window);
//END WINDOW

SDL_Event event;
int running = 1;

SDL_Color color;
generateNoise();
ColorHSL.h=169;
ColorHSL.s=255;
Uint32 fmt=SDL_GetWindowPixelFormat(Window);
temp_surface=SDL_CreateRGBSurfaceWithFormat(0, ww, wh, 32, fmt);
double t;
//END   INIT

//BEGIN MAIN LOOP
while(running){
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	//BEGIN EVENT LOOP
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			running =0;
		}
		if(event.type == SDL_KEYDOWN ){
			switch(event.key.keysym.sym ){
				case SDLK_ESCAPE:
					running =0;
					break;
				case SDLK_s:
					SDL_RenderReadPixels(Renderer, NULL, fmt,temp_surface->pixels,temp_surface->pitch);
					IMG_SaveJPG(temp_surface, "out.jpg", 255);
					break;
				default:
					break;
			}
		}
	}
	//END   EVENT LOOP
	//BEGIN RENDERING
// 	t = SDL_GetTicks()/40.0;
// 	t = fTimeDelta()*400;
	t = fTime()*10.0;
	for(int y = 0; y < noiseHeight; y++)
		for(int x = 0; x < noiseWidth; x++)
			{
				ColorHSL.l = 192 + (Uint8)(turbulence(x, y, t, 32)) / 4;
				color = HSLtoRGB(ColorHSL);
				SDL_SetRenderDrawColor(Renderer, color.r , color.g , color.b, 255);
				SDL_RenderDrawPoint(Renderer, x, y);
				
			}
	SDL_RenderPresent(Renderer);
	//END   RENDERING
}
//END   MAIN LOOP
if (temp_surface!=NULL)
	SDL_FreeSurface(temp_surface);
exit_();
return EXIT_SUCCESS;

}
//END   MAIN FUNCTION

//BEGIN FUNCTIONS
void generateNoise()
{
	for(int z = 0; z < noiseDepth; z++)
		for(int y = 0; y < noiseHeight; y++)
			for(int x = 0; x < noiseWidth; x++)
			{
				noise[z][y][x] = (rand() % RAND_MAX) / (float)RAND_MAX;
			}
}

double smoothNoise(double x, double y, double z)
{
	//get fractional part of x and y
	double fractX = x - (int)(x);
	double fractY = y - (int)(y);
	double fractZ = z - (int)(z);
	
	//wrap around
	int x1 = ((int)(x) + noiseWidth)  % noiseWidth;
	int y1 = ((int)(y) + noiseHeight) % noiseHeight;
	int z1 = ((int)(z) + noiseDepth)  % noiseDepth;
	
	//neighbor values
	int x2 = (x1 + noiseWidth  - 1)   % noiseWidth;
	int y2 = (y1 + noiseHeight - 1)   % noiseHeight;
	int z2 = (z1 + noiseDepth  - 1)   % noiseDepth;
	
	//smooth the noise with bilinear interpolation
	double value = 0.0;
	value += fractX       * fractY       * fractZ       * noise[z1][y1][x1];
	value += fractX       * (1 - fractY) * fractZ       * noise[z1][y2][x1];
	value += (1 - fractX) * fractY       * fractZ       * noise[z1][y1][x2];
	value += (1 - fractX) * (1 - fractY) * fractZ       * noise[z1][y2][x2];
	
	value += fractX       * fractY       * (1 - fractZ) * noise[z2][y1][x1];
	value += fractX       * (1 - fractY) * (1 - fractZ) * noise[z2][y2][x1];
	value += (1 - fractX) * fractY       * (1 - fractZ) * noise[z2][y1][x2];
	value += (1 - fractX) * (1 - fractY) * (1 - fractZ) * noise[z2][y2][x2];
	
	return value;
}

double turbulence(double x, double y, double z, double size)
{
	double value = 0.0, initialSize = size;
	
	while(size >= 1)
	{
		value += smoothNoise(x / size, y / size, z / size) * size;
		size /= 2.0;
	}
	
	return(128.0 * value / initialSize);
}
float fTimeDelta(void)
{
	
	static Uint64 frequency  = 0;
	static Uint64 last;
	static Uint64 current = 0;
	
	if (current==0){
		current    	= 	SDL_GetPerformanceCounter();
		frequency	=	SDL_GetPerformanceFrequency();
		return 0.0f;
	}
	last  			=	SDL_GetPerformanceCounter();
	Uint64 dt = last-current;
	current=last;
	return   (float)dt/ (float)frequency;
	
}
SDL_Color HSLtoRGB(HSL_Color colorHSL)
{
	float r, g, b, h, s, l; // This function works with floats between 0 and 1
	float temp1, temp2, tempr, tempg, tempb;
	h = colorHSL.h / 256.0;
	s = colorHSL.s / 256.0;
	l = colorHSL.l / 256.0;
	
	// If saturation is 0, the color is a shade of gray
	if(s == 0) r = g = b = l;
	
	// If saturation > 0, more complex calculations are needed
	else
	{
		//Set the temporary values
		if(l < 0.5) temp2 =  l * (1 + s);
		else        temp2 = (l + s) - (l * s);
		temp1 = 2 * l - temp2;
		tempr = h + 1.0 / 3.0;
		if(tempr > 1) tempr--;
		tempg = h;
		tempb = h - 1.0 / 3.0;
		if(tempb < 0) tempb++;
		
		//Red
		if(tempr < 1.0 / 6.0) r = temp1 + (temp2 - temp1) * 6.0 * tempr;
		else if(tempr < 0.5) r = temp2;
		else if(tempr < 2.0 / 3.0) r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
		else r = temp1;
		
		//Green
		if(tempg < 1.0 / 6.0) g = temp1 + (temp2 - temp1) * 6.0 * tempg;
		else if(tempg < 0.5) g = temp2;
		else if(tempg < 2.0 / 3.0) g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
		else g = temp1;
		
		//Blue
		if(tempb < 1.0 / 6.0) b = temp1 + (temp2 - temp1) * 6.0 * tempb;
		else if(tempb < 0.5) b = temp2;
		else if(tempb < 2.0 / 3.0) b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
		else b = temp1;
	}
	
	SDL_Color colorRGB;
	colorRGB.r = (int)(r * 255.0);
	colorRGB.g = (int)(g * 255.0);
	colorRGB.b = (int)(b * 255.0);
	colorRGB.a = 255;
	return colorRGB;
}
float fTime(void)
{
	
	static Uint64 start 	 = 0;
	static Uint64 frequency  = 0;
	
	if (start==0){
		start		=	SDL_GetPerformanceCounter();
		frequency	=	SDL_GetPerformanceFrequency();
		return 0.0f;
	}
	Uint64 counter    = 0;
	counter    = SDL_GetPerformanceCounter();
	Uint64 accumulate = counter - start;
	return   (float)accumulate / (float)frequency;
	
}

//END   FUNCTIONS
