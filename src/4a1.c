#include <SDL/SDL.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

/* This macro simplifies accessing a given pixel component on a surface. */
#define pel(surf, x, y, rgb) ((unsigned char *)(surf->pixels))[y*(surf->pitch)+x*3+rgb]

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	int x, y, t;
	
	/* Event information is placed in here */
	SDL_Event event;
	
	/* This will be used as our "handle" to the screen surface */
	SDL_Surface *scr;
	
	SDL_Init(SDL_INIT_VIDEO);
	
	/* Get a 640x480, 24-bit software screen surface */
	scr = SDL_SetVideoMode(512, 512, 24, SDL_SWSURFACE);
	assert(scr);
	
	/* Ensures we have exclusive access to the pixels */
	SDL_LockSurface(scr);
	
	for(y = 0; y < scr->h; y++)
		for(x = 0; x < scr->w; x++){
			/* This is what generates the pattern based on the xy co-ord */
			t = ((x*x + y*y) & 511) - 256;
			if (t < 0)
				t = -(t + 1);
			
			/* Now we write to the surface */
			pel(scr, x, y, 0) = 255 - t; 	//red
			pel(scr, x, y, 1) = t; 		//green
			pel(scr, x, y, 2) = t; 		//blue
		}
	SDL_UnlockSurface(scr);
		
	/* Copies the `scr' surface to the _actual_ screen */
	SDL_UpdateRect(scr, 0, 0, 0, 0);

	while(SDL_WaitEvent(&event)){
		if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			break;
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
