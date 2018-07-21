//BEGIN HEAD
//BEGIN DESCRIPTION

/* Clouds
 * HSLtoRGB
 * mousewheel control (0-255)(turbulence)
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

//END   CPP DEFINITIONS

//BEGIN DATASTRUCTURES
double noise[noiseHeight][noiseWidth]; // noise array
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
double turbulence_factor	= 64;
SDL_Color color;
//BEGIN VISIBLES
//END 	VISIBLES

//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
double 	  smoothNoise(double , double);
void 	  generateNoise(void);
double 	  turbulence(double, double, double);
void 	  draw_clouds(double);
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
SDL_SetWindowTitle(Window, "Clouds");
SDL_ShowWindow(Window);
//END WINDOW

SDL_Event event;
int running = 1;

generateNoise();

ColorHSL.h=169;
ColorHSL.s=255;

draw_clouds(turbulence_factor);
Uint32 fmt=SDL_GetWindowPixelFormat(Window);
temp_surface=SDL_CreateRGBSurfaceWithFormat(0, ww, wh, 32, fmt);
//END   INIT

//BEGIN MAIN LOOP
while(running){

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
		if(event.type == SDL_MOUSEWHEEL)
		{
			if(event.wheel.y > 0) // scroll up
			{
				// Pull up code here!
				turbulence_factor+=0.5;
				if (turbulence_factor >= 255)
					turbulence_factor=255;
				draw_clouds(turbulence_factor);
			}
			else if(event.wheel.y < 0) // scroll down
			{
				// Pull down code here!
				turbulence_factor-=0.5;
				if (turbulence_factor < 0)
					turbulence_factor=0;
				draw_clouds(turbulence_factor);
			}
			SDL_Log("turbulence_factor: %f", turbulence_factor);
		}
	}
	//END   EVENT LOOP
	//BEGIN RENDERING
// No update
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
void generateNoise(void)
{
	for (int y = 0; y < noiseHeight; y++)
		for (int x = 0; x < noiseWidth; x++){
			noise[y][x] = (rand() % RAND_MAX) / (float)RAND_MAX;
		}
}

double smoothNoise(double x, double y)
{
	//get fractional part of x and y
	double fractX = x - (int)x;
	double fractY = y - (int)y;
	
	//wrap around
	int x1 = ((int)x + noiseWidth)  % noiseWidth;
	int y1 = ((int)y + noiseHeight) % noiseHeight;
	
	//neighbor values
	int x2 = (x1 + noiseWidth  - 1) % noiseWidth;
	int y2 = (y1 + noiseHeight - 1) % noiseHeight;
	
	//smooth the noise with bilinear interpolation
	double value = 0.0;
	value += fractX     	* fractY     	* noise[y1][x1];
	value += (1 - fractX) 	* fractY     	* noise[y1][x2];
	value += fractX     	* (1 - fractY) 	* noise[y2][x1];
	value += (1 - fractX) 	* (1 - fractY) 	* noise[y2][x2];
	
	return value;
}

double turbulence(double x, double y, double size)
{
	double value = 0.0, initialSize = size;
	
	while(size >= 1)
	{
		value += smoothNoise(x / size, y / size) * size;
		size /= 2.0;
	}
	
	return(128.0 * value / initialSize);
}

void 	draw_clouds(double tf)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	
	for(int y = 0; y < noiseHeight; y++)
		for(int x = 0; x < noiseWidth; x++){
			ColorHSL.l = 192 + (Uint8)(turbulence(x, y, tf)) / 4;
			color = HSLtoRGB(ColorHSL);
			SDL_SetRenderDrawColor(Renderer, color.r , color.g , color.b, 255);
			SDL_RenderDrawPoint(Renderer, x, y);
		}
		
	SDL_RenderPresent(Renderer);
}
SDL_Color HSLtoRGB(HSL_Color colorHSL)
{
	float r, g, b, h, s, l; //this function works with floats between 0 and 1
	float temp1, temp2, tempr, tempg, tempb;
	h = colorHSL.h / 256.0;
	s = colorHSL.s / 256.0;
	l = colorHSL.l / 256.0;
	
	//If saturation is 0, the color is a shade of gray
	if(s == 0) r = g = b = l;
	
	//If saturation > 0, more complex calculations are needed
	else
	{
		//Set the temporary values
		if(l < 0.5) temp2 = l * (1 + s);
		else temp2 = (l + s) - (l * s);
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
//END   FUNCTIONS
