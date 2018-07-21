//BEGIN HEAD
//BEGIN DESCRIPTION

/* Some Experiments
 * included: HSLtoRGB
 * included: HSVtoRGB
 * included: mousewheel control drawing function
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

#define MAX_DRAWING_FUNCTIONS 8
//END   CPP DEFINITIONS

//BEGIN DATASTRUCTURES
double noise[noiseHeight][noiseWidth]; // noise array
typedef struct HSL_Color
{
	Uint8 h;	// Hue
	Uint8 s;	// Saturation
	Uint8 l;	// Lightness

} HSL_Color;
HSL_Color ColorHSL;

typedef struct HSV_Color
{
	Uint8 h;	// Hue
	Uint8 s;	// Saturation
	Uint8 v;	// Value or B for Brightness
	
} HSV_Color;
HSV_Color ColorHSV;
//END	DATASTRUCTURES

//BEGIN GLOBALS
int ww=WW;
int wh=WH;

SDL_Surface    *temp_surface	= NULL;
double turbulence_factor	= 12;
SDL_Color color;
//BEGIN VISIBLES
//END 	VISIBLES
unsigned int switch_counter=1;
//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
double 	  smoothNoise(double , double);
void 	  generateNoise(void);
double 	  turbulence(double, double, double);
SDL_Color HSLtoRGB(HSL_Color);
SDL_Color HSVtoRGB(HSV_Color colorHSV);
void	  drawing_switch(void);
void 	  draw_1(void);
void 	  draw_2(double tf);
void 	  draw_3(void);
void 	  draw_4(void);
void 	  draw_5(void);
void 	  draw_6(void);
void 	  draw_7(void);
void 	  draw_8(void);

// This one for more testing
void 	  draw_9(void);
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
SDL_SetWindowTitle(Window, "Some Experiments");
SDL_ShowWindow(Window);
//END WINDOW

SDL_Event event;
int running = 1;

generateNoise();

ColorHSL.h=169;
ColorHSL.s=255;

draw_1();
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
				switch_counter++;
				drawing_switch();
			}
			else if(event.wheel.y < 0) // scroll down
			{
				// Pull down code here!
				switch_counter--;
				drawing_switch();
			}
// 			SDL_Log("turbulence_factor: %f", turbulence_factor);
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
SDL_Color HSVtoRGB(HSV_Color colorHSV)
{
	float r, g, b, h, s, v; //this function works with floats between 0 and 1
	h = colorHSV.h / 256.0;
	s = colorHSV.s / 256.0;
	v = colorHSV.v / 256.0;
	
	//If saturation is 0, the color is a shade of gray
	if(s == 0) r = g = b = v;
	//If saturation > 0, more complex calculations are needed
	else
	{
		float f, p, q, t;
		int i;
		h *= 6; //to bring hue to a number between 0 and 6, better for the calculations
		i = (int)(floor(h));  //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
		f = h - i;  //the fractional part of h
		p = v * (1 - s);
		q = v * (1 - (s * f));
		t = v * (1 - (s * (1 - f)));
		switch(i)
		{
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			case 5: r = v; g = p; b = q; break;
		}
	}
	
	SDL_Color colorRGB;
	colorRGB.r = (int)(r * 255.0);
	colorRGB.g = (int)(g * 255.0);
	colorRGB.b = (int)(b * 255.0);
	return colorRGB;
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
void	drawing_switch(void)
{
	if (switch_counter>MAX_DRAWING_FUNCTIONS)
		switch_counter=1;
	if (switch_counter==0)
		switch_counter=MAX_DRAWING_FUNCTIONS;
	
	switch(switch_counter)
	{
		case 1:
			draw_1();
			break;
		case 2:
			draw_2(32);
			break;
		case 3:
			draw_3();
			break;
		case 4:
			draw_4();
			break;
		case 5:
			draw_5();
			break;
		case 6:
			draw_6();
			break;
		case 7:
			draw_7();
			break;
		case 8:
			draw_8();
			break;
// 		case 9:
// 			draw_9();
// 			break;
		default:
			SDL_Log("More Functions defined than handlers. Case not handled. Add Handler.");
			break;
	}
}
void 	draw_1(void)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	for(int y = 0; y < noiseHeight; y++)
		for(int x = 0; x < noiseWidth; x++){

			ColorHSL.l = sin(x) + sin(y);
			color = HSLtoRGB(ColorHSL);
			
			SDL_SetRenderDrawColor(Renderer, color.r , color.g , color.b, 255);
			SDL_RenderDrawPoint(Renderer, x, y);
		}
	SDL_RenderPresent(Renderer);
}
void 	draw_2(double tf)
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
void 	draw_3(void)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	SDL_RenderClear(Renderer);
	//BEGIN GRADIENT
	for(int y = 0; y < noiseHeight; y++){
		for(int x = 0; x < noiseWidth; x++){
			color.r = (Uint8) roundf(         (  255.0/(float)noiseWidth)*(float)x );
			color.g = (Uint8) roundf( 255.0 - ( (255.0/(float)noiseHeight)*(float)y) );
			color.b = (Uint8) roundf( 255.0 - ( (255.0/(float)noiseWidth)*(float)x) );
			SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, 255);
			SDL_RenderDrawPoint(Renderer,x,y);
		}
		
	}
	//END GRADIENT
	SDL_RenderPresent(Renderer);

}
void 	draw_4(void)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	//BEGIN GRADIENT
	for(int y = 0; y < noiseHeight; y++){
		for(int x = 0; x < noiseWidth; x++){
			color.r = (Uint8) roundf(         (  255.0/(float)noiseWidth)*(float)x );
			color.g = 111;
			color.b = 111;
// 			color.g = (Uint8) roundf( 255.0 - ( (255.0/(float)noiseHeight)*(float)y) );
// 			color.b = (Uint8) roundf( 255.0 - ( (255.0/(float)noiseWidth)*(float)x) );
			SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, 255);
			SDL_RenderDrawPoint(Renderer,x,y);
		}
		
	}
	//END GRADIENT
	SDL_RenderPresent(Renderer);
	// 			sin(x) + sin(y)
}
void 	draw_5(void)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	//BEGIN GRADIENT
	for(int y = 0; y < noiseHeight; y++){
		for(int x = 0; x < noiseWidth; x++){
// 			color.r = (Uint8) roundf(         (  255.0/(float)noiseWidth)*(float)x );
			color.r = (Uint8) roundf( 255.0 - ( (255.0/(float)noiseHeight)*(float)y) );
			color.g = 111;
			color.b = 111;
			// 			color.g = (Uint8) roundf( 255.0 - ( (255.0/(float)noiseHeight)*(float)y) );
			// 			color.b = (Uint8) roundf( 255.0 - ( (255.0/(float)noiseWidth)*(float)x) );
			SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, 255);
			SDL_RenderDrawPoint(Renderer,x,y);
		}
		
	}
	//END GRADIENT
	SDL_RenderPresent(Renderer);
}
void 	draw_6(void)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	//BEGIN GRADIENT
	for(int y = 0; y < noiseHeight; y++){
		for(int x = 0; x < noiseWidth; x++){
			color.r = (Uint8) round( sin(x) + sin(y));
			color.g = (Uint8) round( sin(x) + sin(y));
			color.b = 255;
			SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, 255);
			SDL_RenderDrawPoint(Renderer,x,y);
		}
		
	}
	//END GRADIENT
	SDL_RenderPresent(Renderer);
}
void 	draw_7(void)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	double xyPeriod  = 20; 		//number of rings
	double turbPower = 0.05; 	//makes twists
	double turbSize  = 60.0; 	//initial size of the turbulence
	
	
	//BEGIN GRADIENT
	for(int y = 0; y < noiseHeight; y++){
		for(int x = 0; x < noiseWidth; x++){
			
			double xValue = (x - noiseWidth / 2) / (double)(noiseWidth) + turbPower * turbulence(x, y, turbSize) / 256.0;
			double yValue = (y - noiseHeight / 2) / (double)(noiseHeight) + turbPower * turbulence(noiseHeight - y, noiseWidth - x, turbSize) / 256.0;
			double sineValue = 22.0 * fabs(sin(xyPeriod * xValue * 3.1415) + sin(xyPeriod * yValue * 3.1415));
			ColorHSV.h=(Uint8)(sineValue);
			ColorHSV.s=255;
			ColorHSV.v=255;
			color = HSVtoRGB(ColorHSV);
			SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, 255);
			SDL_RenderDrawPoint(Renderer,x,y);
		}
	}
	//END GRADIENT
	SDL_RenderPresent(Renderer);
}
void 	draw_8(void)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	//BEGIN GRADIENT
	for(int y = 0; y < noiseHeight; y++){
		for(int x = 0; x < noiseWidth; x++){
			color.r = (Uint8) roundf(  ( (128.0/(float)noiseWidth)*(float)x) + ( (128.0/(float)noiseHeight)*(float)y)  );
			color.g = 111;
			color.b = 111;
			SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, 255);
			SDL_RenderDrawPoint(Renderer,x,y);
		}
		
	}
	//END GRADIENT
	SDL_RenderPresent(Renderer);
}

void 	draw_9(void)
{
	SDL_SetRenderDrawColor(Renderer, WHITE);
	SDL_RenderClear(Renderer);
	//BEGIN GRADIENT
	for(int y = 0; y < noiseHeight; y++){
		for(int x = 0; x < noiseWidth; x++){
			
			
// 			color.r = (Uint8) roundf( (256/sin(4*(float)x)/(float)x)  );
			
			color.r = (Uint8) roundf(256/sqrt(1-((float)x/(float)noiseHeight*(float)x/(float)noiseHeight)));
			color.g = 111;
			color.b = 111;
			SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, 255);
			SDL_RenderDrawPoint(Renderer,x,y);
		}
		
	}
	//END GRADIENT
	SDL_RenderPresent(Renderer);
}
//END   FUNCTIONS
