#include "area.h"
#include "error.h"

//Area constructor: width, height,pos on screen: x and y,width on screen,height on screen,link to pangoconsole
Area::Area(int w,int h,int sx,int sy,int sw,int sh,PangoConsole& _pc):
	width(w),height(h),screenposx(sx),screenposy(sy),pc(_pc),
	originx(0),originy(0),screenwidth(sw),screenheight(sh),
	pen(White),paper(Black),cursorx(0),cursory(0),ShowCursor(false) {
		//check if the area pos and width on screen is sane
		if(screenposx<0 || screenposy<0 || screenposx+screenwidth>pc.widthchar-1 || screenposy+screenheight>pc.heightchar-1){
			showerror(pc.winhandle,L"Attempt to create an area that lies partly off-screen");
		}

		chargrid=new charcell[width*height];
}

Area::~Area(){
	delete [] chargrid;
}

//can cell x,y be seen on the screen or is it scrooled off?
 bool Area::CellIsVisible(int x,int y){
	if(x>=originx && y>=originy && x < originx+screenwidth && y<originy+screenheight)
		return(true); else return(false);
}
 bool Area::CursorIsVisible(){
	if(CellIsVisible(cursorx,cursory))
		return(true); else return(false);
}

void Area::SetOrigin(int x,int y){
	if(x<0 || y<0 || x+screenwidth>width || y+screenheight>height){
		showerror(pc.winhandle,L"Attempt to set origin of area such that screenarea would not be filled");
	}
	originx=x;originy=y;
	Draw();
}
void Area::Draw(){
	//we can trust that the origin is sensible and the area doesn't go offscreen because that's been checked
	//on creation and on setorigin
	for(auto g=0;g<screenheight;++g){
		for(auto f=0;f<screenwidth;++f){
			charcell* cc=chargrid+(((g+originy)*width)+f+originx);
			pc.drawchar((f)+screenposx,(g)+screenposy,cc->c,cc->fg,cc->bg,true,false);
		}
	}
	
	//this is probably better done in pangoconsole?
	int tempx=pc.fontsizex*pc.zoomx,tempy=pc.fontsizey*pc.zoomy;//TODO surely do these only once at start
	SDL_UpdateRect(pc.screen, screenposx*tempx, screenposy*tempy, screenwidth*tempx,screenheight*tempy);

}

//This is the lowest level drawing for an area: char with 32 bit colour numbers
void Area::DrawChar(unsigned char c,Uint32 fg,Uint32 bg,int x,int y,bool updategrid,bool updatescreen){
	pc.SetCurrentlyActiveArea(this);//do we need this?
	//check if out of bounds here, even before sending on
	if(x>width-1 || x<0 || y<0 || y>height-1){
		showerror(0,L"Area: Attempt to draw a character out of bounds.");
	}
	//set the chargrid regardless of whether character will be visible
	charcell* cc=chargrid+((y*width)+x);
	cc->bg=bg;
	cc->fg=fg;
	cc->c=c;
	//work out if it's on the visible section of the area
	//if it is, pass modified co-ords to the original chardraw
	
	
	//if(x>=originx && y>=originy && x < originx+screenwidth && y<originy+screenheight){
	if(CellIsVisible(x,y))
		pc.drawchar((x-originx)+screenposx,(y-originy)+screenposy,c,fg,bg,updategrid,updatescreen);
	//}
}
//This is as low level as DrawChar, it's just a simple loop of it
void Area::DrawString(std::string s,Uint32 fg,Uint32 bg,int x,int y){
	for(auto c : s){
		DrawChar(c,fg,bg,x++,y);
	}
}

//This draws a string on an area. x and y default to cursor pos,colours default to last used. Wraps; Supports \n.
void Area::PrintString(std::string s,Ecrayola fg,Ecrayola bg,int x,int y){
	if(fg==DEFAULTCOLOUR)fg=pen; else pen=fg;
	if(bg==DEFAULTCOLOUR)bg=paper; else paper=bg;
	if(x==32500)x=cursorx; 
	if(y==32500)y=cursory;
	for(auto c : s){
		if(c=='\n'){//treating \n as CRLF is this ok?
			y++;x=0;
		} else {
			DrawChar(c,crayola[fg],crayola[bg],x++,y);
			if(x==width)x=0,y++;
		}
	}
	cursorx=x;cursory=y;

}

void Area::FlashChar(int x,int y,bool reverse){
	charcell* cc=chargrid+((y*width)+x);
	if(reverse)
		DrawChar(cc->c,crayola[Black],crayola[White],x,y,false);
	else
		DrawChar(cc->c,crayola[White],crayola[Black],x,y,false);
}
void Area::FlashCursor(bool reverse){
	FlashChar(cursorx,cursory,reverse);
}
