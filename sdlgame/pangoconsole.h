#ifndef _pangoconsole_h
#define _pangoconsole_h

#include "font.h"
#include "sdl.h"
#include <string>
#include "colours.h"
#include "windows.h"
//#include "area.h"
class Area;


//This struct represents one character cell on the screen
struct charcell {
		Uint32 fg,bg;
		Uint8 c;
		charcell():fg(0),bg(0),c(32){
		}
	};

//represents the graphical main window of your application
class PangoConsole {
public:
	unsigned char* font;
	Uint8 dataperchar;
	Uint8 zoomx,zoomy; //1 is normal, i.e. 8x8 font is 8x8. 2 is double, i.e. 8x8 font is 16x16
	Uint8 fontsizex,fontsizey;
	int xres,yres; //resolution of screen, x and y, in pixels
	int widthchar,heightchar; //width of screen and height of screen in character cells
	int cursorx,cursory; //current cursor position
	Uint32 pen,paper; //current background and foreground

	HWND winhandle; //window handle for win32 stuff like message boxes and file dialogs

	Area* CurrentlyActiveArea;


	charcell *chargrid; //pointer to start of character cell array
	SDL_Surface* screen; //pointer to sdl screen
	PangoConsole(int x,int y,unsigned char* _font,Uint8 _fontsizex,Uint8 _fontsizey,Uint8 _zoomx=1,Uint8 _zoomy=1,bool fullscreen=false);
	~PangoConsole();
	void SetCurrentlyActiveArea(Area* a);
	void drawchar(int x,int y,unsigned char c,Uint32 fg,Uint32 bg=0,bool updategrid=true,bool updatescreen=true);
	void drawchar(int x,int y,unsigned char c,Ecrayola fg,Ecrayola bg=Ecrayola::Black,bool updategrid=true);

	//void drawchar8(int x,int y,unsigned char c,Uint32 fg,Uint32 bg=0,bool updategrid=true);
	//void drawchar8(int x,int y,unsigned char c,Ecrayola fg,Ecrayola bg=Ecrayola::Black,bool updategrid=true);

	//void drawchar16(int x,int y,unsigned char c,Uint32 fg,Uint32 bg=0,bool updategrid=true);
	//void drawchar16(int x,int y,unsigned char c,Ecrayola fg,Ecrayola bg=Ecrayola::Black,bool updategrid=true);

	void drawcharnobg(int x,int y,unsigned char c,Uint32 fg,bool updategrid=true);
	void drawcharcharonly(int x,int y,unsigned char c,bool updategrid=true);
	void drawcharbgfgonly(int x,int y,Uint32 fg,Uint32 bg,bool updategrid=true);
	void drawstring(int x,int y,std::string s,Uint32 fg,Uint32 bg=0);
	void drawstring(int x,int y,std::string s,Ecrayola fg,Ecrayola bg=Ecrayola::Black);
	void PangoConsole::DrawPixel(int ignorethis,short x,short y,Uint32 colour);
	void PangoConsole::DrawWuLine (short X0, short Y0, short X1, short Y1,Uint8 r,Uint8 g,Uint8 b);
	Uint32 PangoConsole::maskitup(Uint8 r,Uint8 g,Uint8 b,unsigned short w);
	void PangoConsole::drawframe(int x,int y,int w,int h,Uint32 fg,Uint32 bg=0);
	void PangoConsole::drawrect(int x,int y,int x2,int y2,Uint8 character,Uint32 fg,Uint32 bg=0,bool update=true);
	void PangoConsole::drawfilledrect(int x,int y,int x2,int y2,Uint8 character,Uint32 fg,Uint32 bg=0,bool update=true);
	void PangoConsole::redrawrect(int x,int y,int x2,int y2);
	void PangoConsole::redrawfilledrect(int x,int y,int x2,int y2);//PROBABLY NOT NEEDED ANY MORE
	void PangoConsole::drawcard(int x,int y,Uint8 value,Uint8 suit);
	void PangoConsole::testdraw();
	void PangoConsole::redrawchar(int x,int y,bool updatescreen=true);
	void PangoConsole::flashchar(int x,int y,bool reverse=false);
	void PangoConsole::flashmaus(int x,int y,bool reverse=false);
	void PangoConsole::drawsprite(int x,int y,int w,int h,unsigned char* array);
	void PangoConsole::colourgrid(int x,int y);
	void PangoConsole::bresline(int x,int y,int x2,int y2,Uint8 c,Uint32 fg,Uint32 bg=0,bool update=true);
	void PangoConsole::redrawbresline(int x,int y,int x2,int y2);
	void PangoConsole::drawcircle(int x0, int y0, int radius,Uint8 c,Uint32 fg,Uint32 bg=0,bool updategrid=true);
	void PangoConsole::drawfilledcircle(int x0, int y0, int radius,Uint8 c,Uint32 fg,Uint32 bg=0,bool updategrid=true);
	void PangoConsole::redrawcircle(int x0, int y0, int radius);

	void PangoConsole::FlashActiveCursor();
};



#endif

