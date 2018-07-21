//BEGIN HEAD
//BEGIN DESCRIPTION

/* Marble Texture
 */

/* DEFINED PROGRESS GOALS
 * make function			DONE
 * make rendering parametrized:	
 * x,y,p,s				DONE
 * press r to randomize parameters	DONE
 * switch mode: colors - grey
 * 		c - g			DONE
 * 
 * 
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
// xPeriod and yPeriod together define the angle of the lines
// xPeriod and yPeriod both 0 ==> it becomes a normal clouds or turbulence pattern
double xPeriod =  5.0; //defines repetition of marble lines in x direction
double yPeriod = 10.0; //defines repetition of marble lines in y direction
//turbPower = 0 ==> it becomes a normal sine pattern
double turbPower = 5.0; //makes twists
double turbSize = 32.0; //initial size of the turbulence
char colormode=0;
SDL_Color color;
SDL_Color color_factor;
//BEGIN VISIBLES
//END 	VISIBLES

//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
double 	  smoothNoise   (double , double);
void 	  generateNoise (void);
double 	  turbulence	(double, double, double);
void 	  draw_marble	(double, double, double, double);
void randomize_parameters(void);
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
SDL_SetWindowTitle(Window, "Marble Texture");
SDL_ShowWindow(Window);
//END WINDOW

SDL_Event event;
int running = 1;

generateNoise();

draw_marble(xPeriod,yPeriod, turbPower, turbSize);

Uint32   fmt=SDL_GetWindowPixelFormat(Window);
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
				case SDLK_c:
					colormode=1;
					break;
				case SDLK_g:
					colormode=0;
					break;
				case SDLK_i://pure sine
					xPeriod   = 1;
					yPeriod   = 1;

					turbPower = 1;
					turbSize  = 1;
					draw_marble(xPeriod,yPeriod, turbPower, turbSize);
					break;
				case SDLK_j://pure sine
					xPeriod   = 2;
					yPeriod   = 2;
					
					turbPower = 1;
					turbSize  = 1;
					draw_marble(xPeriod,yPeriod, turbPower, turbSize);
					break;
				case SDLK_k://pure sine
					xPeriod   = 0.1;
					yPeriod   = 0.1;
					
					turbPower = 0.01;
					turbSize  = 0.01;
					draw_marble(xPeriod,yPeriod, turbPower, turbSize);
					
					break;
				case SDLK_r:
					randomize_parameters();
					break;
				case SDLK_s:
					SDL_RenderReadPixels(Renderer, NULL, fmt,temp_surface->pixels,temp_surface->pitch);
					IMG_SaveJPG(temp_surface, "out.jpg", 255);
					break;
				default:
					break;
			}
		}
// 		if(event.type == SDL_MOUSEWHEEL)
// 		{
// 			if(event.wheel.y > 0) // scroll up
// 			{
// 				// Pull up code here!
// 				turbulence_factor+=0.5;
// 				if (turbulence_factor >= 255)
// 					turbulence_factor=255;
// 				draw_clouds(turbulence_factor);
// 			}
// 			else if(event.wheel.y < 0) // scroll down
// 			{
// 				// Pull down code here!
// 				turbulence_factor-=0.5;
// 				if (turbulence_factor < 0)
// 					turbulence_factor=0;
// 				draw_clouds(turbulence_factor);
// 			}
// 			SDL_Log("turbulence_factor: %f", turbulence_factor);
// 		}
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

void 	draw_marble(double xPeriod, double yPeriod, double turbPower, double turbSize)
{

	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	
	for(int y = 0; y < noiseHeight; y++)
		for(int x = 0; x < noiseWidth; x++){
			double xyValue = x * xPeriod / noiseWidth + y * yPeriod / noiseHeight + turbPower * turbulence(x, y, turbSize) / 256.0;

			if (colormode){
				double sineValue = color_factor.r * fabs(sin(xyValue * 3.14159));
				color.r = (Uint8)(color_factor.g + sineValue);
				color.g = (Uint8)(color_factor.b + sineValue);
				color.b = (Uint8)(sineValue);
			} else{
				double sineValue = 256 * fabs(sin(xyValue * 3.14159));
				color.r = color.g = color.b = (Uint8)(sineValue);
			}
			SDL_SetRenderDrawColor(Renderer, color.r , color.g , color.b, 255);
			SDL_RenderDrawPoint(Renderer, x, y);
		}
		
	SDL_RenderPresent(Renderer);
}
void randomize_parameters(void)
{
	int min_number= 1;
	int max_number= 10;
	xPeriod=(float)(rand() % (max_number + 1 - min_number) + min_number);
	yPeriod=(float)(rand() % (max_number + 1 - min_number) + min_number);
// 	yPeriod=xPeriod;
// 	xPeriod=((float)rand())/RAND_MAX;
	SDL_Log("xp: %f\n",xPeriod);
// 	yPeriod=((float)rand())/RAND_MAX;
	SDL_Log("yp: %f\n",yPeriod);
	min_number= 1;
	max_number= 128;
	// Decreasing turbPower will give less twists
	turbPower=(float)(rand() % (max_number + 1 - min_number) + min_number);
	SDL_Log("tp: %f\n",turbPower);
	turbSize=(float)(rand() % (max_number + 1 - min_number) + min_number);
	SDL_Log("ts: %f\n",turbSize);
	
	if (colormode){
		min_number= 0;
		max_number= 255;
		color_factor.r=(rand() % (max_number + 1 - min_number) + min_number);
		color_factor.g=(rand() % (max_number + 1 - min_number) + min_number);
		color_factor.b=(rand() % (max_number + 1 - min_number) + min_number);
	}
		
	draw_marble(xPeriod,yPeriod, turbPower, turbSize);
}
//END   FUNCTIONS
