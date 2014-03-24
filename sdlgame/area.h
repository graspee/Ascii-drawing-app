#ifndef _area_h
#define _area_h

#include "pangoconsole.h"

//maybe we store a position the area is being shown on screen and the current origin of the area
//sometihng like area.absoluteset[2*3] vs. area.setWRTscreen[2*3]
//maybe the area can have built in scrollbars that update automatically
//or this could be in a superset class 
//fn to blit from rect of map to area of screen
//now the drawing functions have to update the right part of the panel too though
//luckily it's all in one place- drawchar
//have to decide what events will make panel scroll during program:
//pressing return at the bottom, pressing down arrow at bottom etc.
//how to show areas on screen that can't be edited, i.e. when the window
//has an area without a part of area to show in it
//how to implement infinite scrolling downwards like a text editor



//Represents a scrollable text area on the screen
class Area {
public:
	//width and height in characters of the full thing
	int width,height;
	//pointer to an array of charcells holding the character and colour information
	charcell* chargrid; 
	//origin of area shown on screen, starts at 0,0 but changes when it scrolls
	int originx,originy;
	//position on the screen where the area is drawn
	int screenposx,screenposy;
	//width and height of the screen window. could be less than the total area
	int screenwidth,screenheight;
	//link to the pangoconsole
	PangoConsole& pc;
	//default or current colours
	Ecrayola pen,paper;
	//where the cursor is, whether you can see it or not
	int cursorx,cursory;
	bool ShowCursor;

	
	Area::Area(int w,int h,int sx,int sy,int sw,int sh,PangoConsole& _pc);
	Area::~Area();
	void Area::SetOrigin(int x,int y);
	void Area::Draw();
	
	 bool Area::CellIsVisible(int x,int y);


	 bool Area::CursorIsVisible();


	void Area::DrawChar(unsigned char c,Uint32 fg,Uint32 bg,int x,int y,bool updategrid=true,bool updatescreen=true);
	void Area::DrawString(std::string s,Uint32 fg,Uint32 bg,int x,int y);
	void Area::PrintString(std::string s,Ecrayola fg=DEFAULTCOLOUR,Ecrayola bg=DEFAULTCOLOUR,int x=32500,int y=32500);
	//void Area::drawcharnobg(int x,int y,unsigned char c,Uint32 fg,bool updategrid=true);
	//void Area::drawcharcharonly(int x,int y,unsigned char c,bool updategrid=true);
	//void Area::drawcharbgfgonly(int x,int y,Uint32 fg,Uint32 bg,bool updategrid=true);
	//
	//void Area::drawstring(int x,int y,std::string s,Ecrayola fg,Ecrayola bg=Ecrayola::Black);
	//void Area::DrawPixel(int ignorethis,short x,short y,Uint32 colour);
	//void Area::DrawWuLine (short X0, short Y0, short X1, short Y1,Uint8 r,Uint8 g,Uint8 b);
	//Uint32 Area::maskitup(Uint8 r,Uint8 g,Uint8 b,unsigned short w);
	//void Area::drawframe(int x,int y,int w,int h,Uint32 fg,Uint32 bg=0);
	//void Area::drawrect(int x,int y,int x2,int y2,Uint8 character,Uint32 fg,Uint32 bg=0,bool update=true);
	//void Area::drawfilledrect(int x,int y,int x2,int y2,Uint8 character,Uint32 fg,Uint32 bg=0,bool update=true);
	//void Area::redrawrect(int x,int y,int x2,int y2);
	//void Area::redrawfilledrect(int x,int y,int x2,int y2);//PROBABLY NOT NEEDED ANY MORE
	//void Area::drawcard(int x,int y,Uint8 value,Uint8 suit);
	//void Area::testdraw();
	//void Area::redrawchar(int x,int y,bool updatescreen=true);
	void Area::FlashChar(int x,int y,bool reverse=false);
	void Area::FlashCursor(bool reverse);
	//void Area::flashmaus(int x,int y,bool reverse=false);
	//void Area::drawsprite(int x,int y,int w,int h,unsigned char* array);
	//void Area::colourgrid(int x,int y);
	//void Area::bresline(int x,int y,int x2,int y2,Uint8 c,Uint32 fg,Uint32 bg=0,bool update=true);
	//void Area::redrawbresline(int x,int y,int x2,int y2);
	//void Area::drawcircle(int x0, int y0, int radius,Uint8 c,Uint32 fg,Uint32 bg=0,bool updategrid=true);
	//void Area::drawfilledcircle(int x0, int y0, int radius,Uint8 c,Uint32 fg,Uint32 bg=0,bool updategrid=true);
	//void Area::redrawcircle(int x0, int y0, int radius);

};





#endif