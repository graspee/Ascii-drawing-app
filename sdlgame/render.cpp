#include "font.h"
#include "sdl.h"
#include <string>
#include "render.h"


SDL_Surface *screen;

void initialize(){

 if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

	

    screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(1);
    }
}

void drawchar(int x,int y,unsigned char c,Uint32 fg,Uint32 bg){
	//lock screen
	 if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }
   
   // Uint32 color = SDL_MapRGB(screen->format, R, G, B);
	Uint32 *bufp;

	x*=8;y*=8;
    bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
    *bufp = c;

	for(int line=0;line<8;line++){
			//for(unsigned char across=0;across<8;across++){
			for(unsigned char across=8;across>0;across--){
				if(amsfont[c][line]&(1<<across)){
					*bufp++=fg;
				} else {
					*bufp++=bg;
				}
			}
		bufp+=(screen->pitch/4)-8;
	}

	//unlock screen
	if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }

	SDL_UpdateRect(screen, x, y, 8, 8);
	
}

void drawstring(int x,int y,std::string s,Uint32 fg,Uint32 bg){
	for(auto c : s){
		drawchar(x++,y,c,fg,bg);
	}
}

 void drawchar(int x,int y,unsigned char c,Ecrayola fg,Ecrayola bg){
	drawchar(x,y,c,crayola[fg],crayola[bg]);
}

 void drawstring(int x,int y,std::string s,Ecrayola fg,Ecrayola bg){
	drawstring(x,y,s,crayola[fg],crayola[bg]);
}
