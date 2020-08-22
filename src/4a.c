//BEGIN HEAD
//BEGIN DESCRIPTION

/* 2d software LUT deformations and tunnels
 * 
 * Repeat Texture / tiling
 * 
 * No Look Up Tabels yet
 *
 * Intro:
 * More sophisticated pixel/texel manipulation
 * Diving deeper into surfaces.
 * based on: https://github.com/Acry/SDL2-Surfaces
 * && https://lodev.org/cgtutor/randomnoise.html
 * && http://www.iquilezles.org/www/articles/deform/deform.htm
 * 
 * More useful equations (u and v are usually texture coordinates):
 * u = x*cos(2**r) - y*sin(2*r) 	v = y*cos(2*r) + x*sin(2*r)
 * u = 0.3/(r+0.5*x)		 	v = 3*a/pi
 * u = 0.02*y+0.03*cos(a*3)/r		v = 0.02*x+0.03*sin(a*3)/r
 * u = 0.1*x/(0.11+r*0.5)		v = 0.1*y/(0.11+r*0.5)
 * u = 0.5*a/pi				v = sin(7*r)
 * u = r*cos(a+r)			v = r*sin(a+r)
 * u = 1/(r+0.5+0.5*sin(5*a))	        v = a*3/pi
 * u = x/abs(y)				v = 1/abs(y)
 * 
 * 
 * Essence:
 * This are foundations for C-Programmers that don't know much about GLSL.
 * Software Rendering/ Bit flipping on CPU-Surfaces is barely done today,
 * cause the GPU-Speed is tremendous.
 * 
 * GLSL-Code is a follow up in https://github.com/Acry/SDL2-OpenGL
 *
 * In the different WipeSurface functions I use different methods to access
 * the Pixels. NOTE that it may not be portable. Not trying to implement an
 * API here.
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
#include "helper_soft.h"
#include <stdlib.h>
//END   INCLUDES

//BEGIN CPP DEFINITIONS
#define WHITE 255,255,255,255
#define WW 1920
#define WH 1080

#define noiseWidth  512
#define noiseHeight 512

/* This macro simplifies accessing a given pixel component on a surface. */
#define SET_PIXEL(surf, x, y, rgba) ((unsigned char *)(surf->pixels))[y*(surf->pitch)+x*4+rgba]
//END   CPP DEFINITIONS

//BEGIN DATASTRUCTURES
double noise[noiseHeight][noiseWidth]; // noise array
//END	DATASTRUCTURES

//BEGIN GLOBALS
int ww=WW;
int wh=WH;

SDL_Surface    *temp_surface	= NULL;
SDL_Surface    *texture		= NULL;
SDL_Surface    *screenbuffer	= NULL;

double xyPeriod  = 20; 		//number of rings
double turbPower = 0.05; 	//makes twists
double turbSize  = 60.0; 	//initial size of the turbulence

SDL_Rect texture_rect={0,0,noiseWidth, noiseHeight};
SDL_Color color;
//BEGIN VISIBLES
//END 	VISIBLES

//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
double 	  smoothNoise		(double , double);
void 	  generateNoise		(void);
double 	  turbulence		(double, double, double);
void 	  draw_wood		(double,double,double);
void 	  renderDeformation	(SDL_Surface *, float);
void 	  putpixel		(SDL_Surface *, int, int, Uint32);
void 	  WipeSurface		(SDL_Surface *);
void 	  WipeSurface2		(SDL_Surface *, int, int, int, int);
void 	  WipeSurface3		(SDL_Surface *, int, int, int, int);
void 	  WipeSurface4		(SDL_Surface *, int, int, int, int);
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
SDL_SetWindowPosition	(Window,0,0);
SDL_SetWindowSize	(Window,ww,wh);
SDL_SetWindowTitle	(Window, "2d deformations");
SDL_ShowWindow		(Window);
//END WINDOW

SDL_Event event;
int running = 1;

generateNoise();

Uint32 fmt	= SDL_GetWindowPixelFormat(Window);
screenbuffer	= SDL_CreateRGBSurfaceWithFormat(0, ww, wh, 32, fmt);
screenbuffer	= SDL_GetWindowSurface(Window);
texture		= SDL_CreateRGBSurfaceWithFormat(0, noiseWidth, noiseHeight, 32, fmt);
temp_surface	= SDL_CreateRGBSurfaceWithFormat(0, ww, wh, 32, fmt);
// draw_wood(xyPeriod ,turbPower, turbSize);
// WipeSurface(texture);
// #if 0
int x, y, t;
for(y = 0; y < texture->h; y++)
	for(x = 0; x < texture->w; x++)
	{
		/* This is what generates the pattern based on the xy co-ord */
		t = ((x*x + y*y) & 511) - 256;
		
		if (t < 0)
			t = -(t + 1);
		
		/* Write to the surface */
		SET_PIXEL(texture, x, y, 0) = t;	 	// red
		SET_PIXEL(texture, x, y, 1) = t; 		// green
		SET_PIXEL(texture, x, y, 2) = 255 - t; 		// blue
		SET_PIXEL(texture, x, y, 3) = 255;		// alpha
	}
// WipeSurface2(texture, 255, 166, 0, 255);
// WipeSurface3(texture, 255, 166, 0, 255);
WipeSurface4(screenbuffer, 255, 0, 0, 255); 	// screen red
// WipeSurface4(texture, 0, 0, 255, 255);		// texture blue
renderDeformation(texture,0);			// fill buffer with texture
// #endif
// SDL_BlitSurface(texture,NULL,screenbuffer,NULL);

SDL_UpdateWindowSurface(Window);

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
					SDL_BlitSurface(screenbuffer,NULL,temp_surface,NULL);
					IMG_SaveJPG(temp_surface, "out.jpg", 255);
					break;
				default:
					break;
			}
		}

	}
	//END   EVENT LOOP
	//BEGIN RENDERING

	//END   RENDERING
}
//END   MAIN LOOP

if (temp_surface!=NULL)
	SDL_FreeSurface(temp_surface);
if (texture!=NULL)
	SDL_FreeSurface(texture);
if (screenbuffer!=NULL)
	SDL_FreeSurface(screenbuffer);

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

void draw_wood(double xyPeriod ,double turbPower,double turbSize)
{
	for(int y = 0; y < noiseHeight; y++)
		for(int x = 0; x < noiseWidth; x++){
			double xValue = (x - noiseWidth / 2)  / (double)(noiseWidth);
			double yValue = (y - noiseHeight / 2) / (double)(noiseHeight);
			double distValue = sqrt(xValue * xValue + yValue * yValue) + turbPower * turbulence(x, y, turbSize) / 256.0;
			double sineValue = 128.0 * fabs(sin(2 * xyPeriod * distValue * 3.14159));
			color.r = (Uint8)(80 + sineValue);
			color.g = (Uint8)(30 + sineValue);
			color.b = 30;
			Uint32 pixel;
			pixel=SDL_MapRGBA(texture->format, color.r , color.g , color.b, 255);
			putpixel(texture, x,y, pixel);
		}

}

void renderDeformation( SDL_Surface *texture, float time )
{
	/* Texture Repeat
	 *
	 * Take the screen buffer and map a texture to it.
	 * texture is of 512 x 512 pixels,
	 * screen  is of ww x wh pixels.
	 * 
	 * This code just copies one texel to one pixel.
	 * 
	 */
	(void) time;
	int bbpp = screenbuffer->format->BytesPerPixel;
	int tbpp = texture->format->BytesPerPixel;

	int ty=0;
	int tx=0;
	
	for(int by = 0; by < screenbuffer->h; by++){
		for(int bx = 0; bx < screenbuffer->w; bx++){
			// get buffer pixels
			Uint32 *bpa = (Uint32 *)screenbuffer->pixels + (by * screenbuffer->pitch/bbpp) + bx;
			// get texture pixels
			Uint32 *tpa = (Uint32 *)     texture->pixels + (ty *      texture->pitch/tbpp) + tx;
			// copy pixel from tex to screenbuffer
			*bpa=*tpa;
			
			tx++;
			if (tx>texture->w-1||bx==screenbuffer->w-1){
				tx=0;
			}
		}
		ty++;
		if (ty>texture->h-1){
			ty=0;
		}
	}

}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	/* Code from 
	 * http://sdl.beuc.net/sdl.wiki/Pixel_Access
	 */
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
	switch(bpp) {
		case 1:
			*p = pixel;
			break;
			
		case 2:
			*(Uint16 *)p = pixel;
			break;
			
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;
			
		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

/* This is meant to show how to edit a surface's pixels on the CPU, but
 *  normally you should use SDL_FillRect() to wipe a surface's contents. */
void WipeSurface(SDL_Surface *surface)
{
	// https://wiki.libsdl.org/SDL_Surface
	
	/* This is fast for surfaces that don't require locking. */
	/* Once locked, surface->pixels is safe to access. */
	// SDL_LockSurface(surface);
	/* This assumes that color value zero is black. Use
	 *      SDL_MapRGBA() for more robust surface color mapping! */
	
	/* height times pitch is the size of the surface's whole buffer. */
	SDL_memset(surface->pixels, 0, surface->h * surface->pitch);
	
// 	SDL_UnlockSurface(surface);
}

// Access with Macro
void WipeSurface2(SDL_Surface *surface, int r, int g, int b, int a)
{
	// FIXME auto adjust RBGA-Order
for(int y = 0; y < surface->h; y++)
	for(int x = 0; x < surface->w; x++){
		SET_PIXEL(surface, x, y, 0) = b;	 	// blue
		SET_PIXEL(surface, x, y, 1) = g; 		// green
		SET_PIXEL(surface, x, y, 2) = r; 		// red
		SET_PIXEL(surface, x, y, 3) = a;		// alpha
	}

}

// Access without Macro
void WipeSurface3(SDL_Surface *surface, int r, int g, int b, int a)
{
	
	// FIXME auto adjust RBGA-Order
	int bpp = surface->format->BytesPerPixel;
	for(int y = 0; y < surface->h; y++)
		for(int x = 0; x < surface->w; x++){
			Uint8 *p = (Uint8 *)surface->pixels + y * (surface->pitch) + x * bpp+0;
			*p = b;
			p = (Uint8 *)surface->pixels + y * (surface->pitch) + x * bpp+1;
			*p = g;
			p = (Uint8 *)surface->pixels + y * (surface->pitch) + x * bpp+2;
			*p = r;
			p = (Uint8 *)surface->pixels + y * (surface->pitch) + x * bpp+3;
			*p = a;
		}
		
}

// using SDL_MapRGBA - writing 4 Bit
void WipeSurface4(SDL_Surface *surface, int r, int g, int b, int a)
{
	
	Uint32 pixel;
	pixel=SDL_MapRGBA(surface->format, r , g , b, a);
	int bpp = surface->format->BytesPerPixel;
	for(int y = 0; y < surface->h; y++)
		for(int x = 0; x < surface->w; x++){
			Uint8 *p = (Uint8 *)surface->pixels + y * (surface->pitch) + x * bpp;
			*(Uint32 *)p = pixel;
		}
		
}
//END   FUNCTIONS
