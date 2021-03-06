//BEGIN HEAD
//BEGIN DESCRIPTION

/* Generate Noise Texture
 * The essence
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

//BEGIN VISIBLES
//END 	VISIBLES

//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
void generateNoise();
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
SDL_SetWindowTitle(Window, "SDL2 - Noise");
SDL_ShowWindow(Window);
//END WINDOW

SDL_Event event;
int running = 1;

SDL_Color color;
SDL_SetRenderDrawColor(Renderer, WHITE);
SDL_RenderClear(Renderer);
generateNoise();
for(int y = 0; y < noiseHeight; y++)
	for(int x = 0; x < noiseWidth; x++){
		color.r = color.g = color.b = (Uint8)(256 * noise[x][y]);
		SDL_SetRenderDrawColor(Renderer, color.r , color.g , color.b, 255);
		SDL_RenderDrawPoint(Renderer, x, y);
	}

SDL_RenderPresent(Renderer);
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
				default:
					break;
			}
		}
	}
	//END   EVENT LOOP
	//BEGIN RENDERING
// No update
	//END   RENDERING
}
//END   MAIN LOOP
exit_();
return EXIT_SUCCESS;

}
//END   MAIN FUNCTION

//BEGIN FUNCTIONS
void generateNoise()
{
	for (int y = 0; y < noiseHeight; y++)
		for (int x = 0; x < noiseWidth; x++){
			noise[y][x] = (rand() % RAND_MAX) / (float)RAND_MAX;
		}
}
//END   FUNCTIONS
