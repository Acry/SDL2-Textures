//BEGIN HEAD
//BEGIN DESCRIPTION

/* add turbulence
 * mousewheel control (0-255)
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
double 	smoothNoise(double , double);
void 	generateNoise(void);
double 	turbulence(double, double, double);
void 	draw_noise(double);
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
SDL_SetWindowTitle(Window, "Turbulence - Wheelcontrol");
SDL_ShowWindow(Window);
//END WINDOW

SDL_Event event;
int running = 1;

generateNoise();
color.a=255;
draw_noise(turbulence_factor);
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
				draw_noise(turbulence_factor);
			}
			else if(event.wheel.y < 0) // scroll down
			{
				// Pull down code here!
				turbulence_factor-=0.5;
				if (turbulence_factor < 0)
					turbulence_factor=0;
				draw_noise(turbulence_factor);
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

void 	draw_noise(double tf)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	for(int y = 0; y < noiseHeight; y++)
		for(int x = 0; x < noiseWidth; x++){
			// 		color.r = color.g = color.b = (Uint8)(256 * noise[x][y]);
			color.r = color.g = color.b = (Uint8)(256 * smoothNoise(x / 8.0, y / 8.0));
			color.r = color.g = color.b = (Uint8)(turbulence(x, y, tf));
			SDL_SetRenderDrawColor(Renderer, color.r , color.g , color.b, 255);
			SDL_RenderDrawPoint(Renderer, x, y);
		}
		
	SDL_RenderPresent(Renderer);
}
//END   FUNCTIONS
