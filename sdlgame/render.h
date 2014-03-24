#ifndef __render_h
#define __render_h

#include "font.h"
#include "sdl.h"
#include <string>
#include "colours.h"

extern SDL_Surface *screen;

void initialize();

void  drawchar(int x,int y,unsigned char c,Uint32 fg,Uint32 bg=0);

void drawstring(int x,int y,std::string s,Uint32 fg,Uint32 bg=0);

 void drawchar(int x,int y,unsigned char c,Ecrayola fg,Ecrayola bg=Ecrayola::Black);

 void drawstring(int x,int y,std::string s,Ecrayola fg,Ecrayola bg=Ecrayola::Black);

//void ShowBMP(char *file, SDL_Surface *screen, int x, int y);

//void DrawPixel(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B,int x,int y);

#endif