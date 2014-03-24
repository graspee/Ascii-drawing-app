

#include "font.h"
#include "sdl.h"
#include <string>
#include "pangoconsole.h"

#include "error.h"
#include <iostream>
#include <fstream>
//#include <ios>
//#include <string>
#include "playingcards.h"
#include "area.h"

PangoConsole::PangoConsole(int x,int y,unsigned char* _font,Uint8 _fontsizex,Uint8 _fontsizey,Uint8 _zoomx,Uint8 _zoomy,bool fullscreen)	:
	xres(x), yres(y),widthchar(xres/(fontsizex*_zoomx)),heightchar(yres/(fontsizey*_zoomy)),cursorx(0), CurrentlyActiveArea(0),
	cursory(0), zoomx(_zoomx),zoomy(_zoomy),fontsizex(_fontsizex),fontsizey(_fontsizey),font(_font),dataperchar(((_fontsizex/8)*(_fontsizex/8))*8){
		//WARNING! dataperchar assumes font is square
 if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		showerror(0,L"Unable to init SDL");
        exit(1);
    }
    atexit(SDL_Quit);

	
	if(fullscreen){
		screen = SDL_SetVideoMode(xres, yres, 32, SDL_SWSURFACE|SDL_FULLSCREEN);
	} else {
		screen = SDL_SetVideoMode(xres, yres, 32,SDL_SWSURFACE );//SDL_DOUBLEBUF|SDL_HWSURFACE
		//screen = SDL_SetVideoMode(xres, yres, 32,SDL_DOUBLEBUF|SDL_HWSURFACE );
	}

    if ( screen == NULL ) {
        fprintf(stderr, "Unable to set video mode video: %s\n", SDL_GetError());
        showerror(0,L"Unable to set video mode");
		exit(1);
    }

	chargrid=new charcell[widthchar*heightchar];

	SetupColours(screen);
	pen=crayola[White]; paper=crayola[Black];

	//get window handle
	winhandle=get_sdl_window_handle();
}
PangoConsole::~PangoConsole(){
	delete [] chargrid;
}
void PangoConsole::SetCurrentlyActiveArea(Area* a){
	CurrentlyActiveArea=a;
}
void PangoConsole::drawchar(int x,int y,unsigned char c,Uint32 fg,Uint32 bg,bool updategrid,bool updatescreen){
	
#ifdef __bounds
	if(x<0 || y<0 || x>widthchar-1 || y>heightchar-1){
			showerror(winhandle,L"PangoConsole: Attempt to draw a character out of bounds.");
	}
#endif
	//if(c==8)c=32;
	//update the character grid
	if(updategrid){
		charcell* cc=chargrid+((y*widthchar)+x);
		cc->bg=bg;
		cc->fg=fg;
		cc->c=c;
	}

	//lock screen
	 if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }
   
	Uint32 *bufp;

	//calc these only once
	Uint8 add1=(fontsizex==8)?1:2; //ugh assumes square font
	Uint8 add2=(fontsizex==8)?0:1;

	x*=fontsizex*zoomx;y*=fontsizey*zoomy; //old ver was *=8 twice
    bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
    *bufp = c;

	for(int line=0;line<fontsizey;line++){ //old ver was <8
		for(Uint8 repeety=0;repeety<zoomy;repeety++){
			for(char across=fontsizex-1;across>-1;across--){ //was across=7
					//new for zoomversion the repeet loop
					for (Uint8 repeetx=0;repeetx<zoomx;repeetx++){
						//if(font[c][line]&(1<<across)){//amsfont
						//if(  *(font+(c*dataperchar)+line)&(1<<(across%8))  ){ //works for 8x8
						//if(  *(font+(c*dataperchar)+line*2+(1?(across<8):0))&(1<<(across%8))  ){//works for 16x16 brcks may be wrong
						if(  *(font+(c*dataperchar)+line*add1+((1?(across<8):0)*add2))&(1<<(across%8))  ){
							*bufp++=fg;
						} else {
							*bufp++=bg;
						}
					} //repeetx
			}//across
		
		bufp+=(screen->pitch/4)-(fontsizex*zoomx); //was -8 on the end
	} //repeety
	}//line

	//unlock screen
	if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }

	if(updatescreen)SDL_UpdateRect(screen, x, y, fontsizex*zoomx, fontsizey*zoomy); //was screen,x,y,8,8


	
}

//draws the specified character with specified fg and bg that is there now
void PangoConsole::drawcharnobg(int x,int y,unsigned char c,Uint32 fg,bool updategrid){
	charcell* cc=chargrid+((y*widthchar)+x);
	drawchar(x,y,c,fg,cc->bg,updategrid);
}
//draws the specified character in the cell with the same fg and bg that is there now
void PangoConsole::drawcharcharonly(int x,int y,unsigned char c,bool updategrid){
	charcell* cc=chargrid+((y*widthchar)+x);
	drawchar(x,y,c,cc->fg,cc->bg,updategrid);
}
//draws the char that already exists in the cell with the specified bg and fg
void PangoConsole::drawcharbgfgonly(int x,int y,Uint32 fg,Uint32 bg,bool updategrid){
	charcell* cc=chargrid+((y*widthchar)+x);
	drawchar(x,y,cc->c,fg,bg,updategrid);
}
void PangoConsole::drawstring(int x,int y,std::string s,Uint32 fg,Uint32 bg){
	for(auto c : s){
		drawchar(x++,y,c,fg,bg);
	}
}

 void PangoConsole::drawchar(int x,int y,unsigned char c,Ecrayola fg,Ecrayola bg,bool updategrid){
	drawchar(x,y,c,crayola[fg],crayola[bg],updategrid);
}

 void PangoConsole::drawstring(int x,int y,std::string s,Ecrayola fg,Ecrayola bg){
	drawstring(x,y,s,crayola[fg],crayola[bg]);
}

 
 
 void PangoConsole::DrawPixel(int ignorethis,short x,short y,Uint32 colour){
	 drawchar(x,y,' ',colour,colour);
 }

 Uint32 PangoConsole::maskitup(Uint8 r,Uint8 g,Uint8 b,unsigned short w){
	 if(w>256)showerror(winhandle,L"w>256 in maskitup");
	 w=256-w;
	 double temp=(double)(256)/(double)w;
	 double dr=(double)r/temp;
	 double dg=(double)g/temp;
	 double db=(double)b/temp;
	 return SDL_MapRGB(screen->format,(Uint8)dr,(Uint8)dg,(Uint8)db);
 }

 void PangoConsole::DrawWuLine (short X0, short Y0, short X1, short Y1,
         Uint8 r,Uint8 g,Uint8 b)
{
	int pDC=0; //dummy value for the fake drawpixel
	short NumLevels=256;
	unsigned short IntensityBits=8;
	Uint32 BaseColor=SDL_MapRGB(screen->format,r,g,b);

   unsigned short IntensityShift, ErrorAdj, ErrorAcc;
   unsigned short ErrorAccTemp, Weighting, WeightingComplementMask;
   short DeltaX, DeltaY, Temp, XDir;

   /* Make sure the line runs top to bottom */
   if (Y0 > Y1) {
      Temp = Y0; Y0 = Y1; Y1 = Temp;
      Temp = X0; X0 = X1; X1 = Temp;
   }
   /* Draw the initial pixel, which is always exactly intersected by
      the line and so needs no weighting */
   DrawPixel(pDC,X0, Y0, BaseColor);

   if ((DeltaX = X1 - X0) >= 0) {
      XDir = 1;
   } else {
      XDir = -1;
      DeltaX = -DeltaX; /* make DeltaX positive */
   }
   /* Special-case horizontal, vertical, and diagonal lines, which
      require no weighting because they go right through the center of
      every pixel */
   if ((DeltaY = Y1 - Y0) == 0) {
      /* Horizontal line */
      while (DeltaX-- != 0) {
         X0 += XDir;
         DrawPixel(pDC,X0, Y0, BaseColor);
      }
      return;
   }
   if (DeltaX == 0) {
      /* Vertical line */
      do {
         Y0++;
         DrawPixel(pDC,X0, Y0, BaseColor);
      } while (--DeltaY != 0);
      return;
   }
   if (DeltaX == DeltaY) {
      /* Diagonal line */
      do {
         X0 += XDir;
         Y0++;
         DrawPixel(pDC,X0, Y0, BaseColor);
      } while (--DeltaY != 0);
      return;
   }
   /* Line is not horizontal, diagonal, or vertical */
   ErrorAcc = 0;  /* initialize the line error accumulator to 0 */
   /* # of bits by which to shift ErrorAcc to get intensity level */
   IntensityShift = 16 - IntensityBits;
   /* Mask used to flip all bits in an intensity weighting, producing the
      result (1 - intensity weighting) */
   WeightingComplementMask = NumLevels - 1;
   /* Is this an X-major or Y-major line? */
   if (DeltaY > DeltaX) {
      /* Y-major line; calculate 16-bit fixed-point fractional part of a
         pixel that X advances each time Y advances 1 pixel, truncating the
         result so that we won't overrun the endpoint along the X axis */
      ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
      /* Draw all pixels other than the first and last */
      while (--DeltaY) {
         ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
         ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
         if (ErrorAcc <= ErrorAccTemp) {
            /* The error accumulator turned over, so advance the X coord */
            X0 += XDir;
         }
         Y0++; /* Y-major, so always advance Y */
         /* The IntensityBits most significant bits of ErrorAcc give us the
            intensity weighting for this pixel, and the complement of the
            weighting for the paired pixel */
         Weighting = ErrorAcc >> IntensityShift;
         DrawPixel(pDC,X0, Y0,maskitup(r,g,b,Weighting)); //basecolor + weighting
         DrawPixel(pDC,X0 + XDir, Y0,
               maskitup(r,g,b, (Weighting ^ WeightingComplementMask))); //basecolor + (weighting ^ weightingcomplementmask)
      }
      /* Draw the final pixel, which is always exactly intersected by the line
         and so needs no weighting */
      DrawPixel(pDC,X1, Y1, BaseColor);
      return;
   }
   /* It's an X-major line; calculate 16-bit fixed-point fractional part of a
      pixel that Y advances each time X advances 1 pixel, truncating the
      result to avoid overrunning the endpoint along the X axis */
   ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
   /* Draw all pixels other than the first and last */
   while (--DeltaX) {
      ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
      ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
      if (ErrorAcc <= ErrorAccTemp) {
         /* The error accumulator turned over, so advance the Y coord */
         Y0++;
      }
      X0 += XDir; /* X-major, so always advance X */
      /* The IntensityBits most significant bits of ErrorAcc give us the
         intensity weighting for this pixel, and the complement of the
         weighting for the paired pixel */
      Weighting = ErrorAcc >> IntensityShift;
	  DrawPixel(pDC,X0, Y0,maskitup(r,g,b,Weighting)); //BaseColor + Weighting
      DrawPixel(pDC,X0, Y0 + 1,
           maskitup(r,g,b, (Weighting ^ WeightingComplementMask))); //BaseColor + (Weighting ^ WeightingComplementMask)
   }
   /* Draw the final pixel, which is always exactly intersected by the line
      and so needs no weighting */
   DrawPixel(pDC,X1, Y1, BaseColor);
}

void PangoConsole::drawframe(int x,int y,int w,int h,Uint32 fg,Uint32 bg){
	
	//some sanity check that frame won't clip
	if(x<0 || x>widthchar-w || y<0 || y>heightchar-h){
		showerror(winhandle,L"frame off screen");
	}

	drawchar(x,y,218,fg,bg); //top left corner
	for(int f=x+1;f<x+w-1;f++){
		drawchar(f,y,196,fg,bg); // top or bottom edge 154
		drawchar(f,y+h-1,196,fg,bg);
	}
	drawchar(x+w-1,y,191,fg,bg); // top right corner
	for(int f=y+1;f<y+h-1;f++){
		drawchar(x,f,179,fg,bg); // left or right edge
		drawchar(x+w-1,f,179,fg,bg); // left or right edge
	}
	drawchar(x,y+h-1,192,fg,bg); //bottom left corner
	drawchar(x+w-1,y+h-1,217,fg,bg); // bottom right corner 
}
void PangoConsole::redrawrect(int x,int y,int x2,int y2){
	int swap;
	if(x2<x){swap=x;x=x2;x2=swap;}
	if(y2<y){swap=y;y=y2;y2=swap;}

	//some sanity check that rect won't clip
	if(x<0 || x2>widthchar-1 || y<0 || y2>heightchar-1){
		showerror(winhandle,L"rect off screen");
	}

	for(int f=x;f<x2+1;f++){
		redrawchar(f,y);
		redrawchar(f,y2);
	}
	for(int g=y+1;g<y2;g++){
		redrawchar(x,g);
		redrawchar(x2,g);
	}
	
}
void PangoConsole::drawrect(int x,int y,int x2,int y2,Uint8 character,Uint32 fg,Uint32 bg,bool update){
	int swap;
	if(x2<x){swap=x;x=x2;x2=swap;}
	if(y2<y){swap=y;y=y2;y2=swap;}

	//some sanity check that rect won't clip
	if(x<0 || x2>widthchar-1 || y<0 || y2>heightchar-1){
		showerror(winhandle,L"rect off screen");
	}

	for(int f=x;f<x2+1;f++){
		drawchar(f,y,character,fg,bg,update);
		drawchar(f,y2,character,fg,bg,update);
	}
	for(int g=y+1;g<y2;g++){
		drawchar(x,g,character,fg,bg,update);
		drawchar(x2,g,character,fg,bg,update);
	}
	
}
void PangoConsole::drawfilledrect(int x,int y,int x2,int y2,Uint8 character,Uint32 fg,Uint32 bg,bool update){
	int swap;
	if(x2<x){swap=x;x=x2;x2=swap;}
	if(y2<y){swap=y;y=y2;y2=swap;}

	//some sanity check that rect won't clip
	if(x<0 || x2>widthchar-1 || y<0 || y2>heightchar-1){
		showerror(winhandle,L"rect off screen");
	}
	for(int f=x;f<x2+1;f++){
		for(int g=y;g<y2+1;g++){
			drawchar(f,g,character,fg,bg,update,false);//,false
		}
	}
	int tempx=fontsizex*zoomx,tempy=fontsizey*zoomy;
	SDL_UpdateRect(screen, x*tempx, y*tempy, (x2-(x-1))*tempx,(y2-(y-1))*tempy);
}
//PROBABLY NOT NEEDED ANY MORE
void PangoConsole::redrawfilledrect(int x,int y,int x2,int y2){

	int swap;
	if(x2<x){swap=x;x=x2;x2=swap;}
	if(y2<y){swap=y;y=y2;y2=swap;}

	//some sanity check that rect won't clip
	if(x<0 || x2>widthchar-1 || y<0 || y2>heightchar-1){
		showerror(winhandle,L"rect off screen");
	}
	for(int f=x;f<x2+1;f++){
		for(int g=y;g<y2+1;g++){
			redrawchar(f,g,false);
		}
	}
	int tempx=fontsizex*zoomx,tempy=fontsizey*zoomy;
	SDL_UpdateRect(screen, x*tempx, y*tempy, (x2-(x-1))*tempx,(y2-(y-1))*tempy);
}
void PangoConsole::redrawcircle(int x0, int y0, int radius)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  redrawchar(x0, y0 + radius);
  redrawchar(x0, y0 - radius);
  redrawchar(x0 + radius, y0);
  redrawchar(x0 - radius, y0);
 
  while(x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    redrawchar(x0 + x, y0 + y);
    redrawchar(x0 - x, y0 + y);
    redrawchar(x0 + x, y0 - y);
    redrawchar(x0 - x, y0 - y);
    redrawchar(x0 + y, y0 + x);
    redrawchar(x0 - y, y0 + x);
    redrawchar(x0 + y, y0 - x);
    redrawchar(x0 - y, y0 - x);
  }
}
void PangoConsole::drawcircle(int x0, int y0, int radius,Uint8 c,Uint32 fg,Uint32 bg,bool updategrid)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  drawchar(x0, y0 + radius,c,fg,bg,updategrid);
  drawchar(x0, y0 - radius,c,fg,bg,updategrid);
  drawchar(x0 + radius, y0,c,fg,bg,updategrid);
  drawchar(x0 - radius, y0,c,fg,bg,updategrid);
 
  while(x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    drawchar(x0 + x, y0 + y,c,fg,bg,updategrid);
    drawchar(x0 - x, y0 + y,c,fg,bg,updategrid);
    drawchar(x0 + x, y0 - y,c,fg,bg,updategrid);
    drawchar(x0 - x, y0 - y,c,fg,bg,updategrid);
    drawchar(x0 + y, y0 + x,c,fg,bg,updategrid);
    drawchar(x0 - y, y0 + x,c,fg,bg,updategrid);
    drawchar(x0 + y, y0 - x,c,fg,bg,updategrid);
    drawchar(x0 - y, y0 - x,c,fg,bg,updategrid);
  }
}
void PangoConsole::drawfilledcircle(int x0, int y0, int radius,Uint8 c,Uint32 fg,Uint32 bg,bool updategrid)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  drawchar(x0, y0 + radius,c,fg,bg,updategrid);
  drawchar(x0, y0 - radius,c,fg,bg,updategrid);
  bresline(x0 + radius, y0,x0 - radius, y0,c,fg,bg,updategrid);
 
  while(x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    bresline(x0 + x, y0 + y,x0 - x, y0 + y,c,fg,bg,updategrid);
    bresline(x0 + x, y0 - y,x0 - x, y0 - y,c,fg,bg,updategrid);
    bresline(x0 + y, y0 + x,x0 - y, y0 + x,c,fg,bg,updategrid);
    bresline(x0 + y, y0 - x,x0 - y, y0 - x,c,fg,bg,updategrid);
  }
}

void PangoConsole::drawcard(int x,int y,Uint8 value,Uint8 suit){
	static const unsigned char rankchar[13]={'A','2','3','4','5','6','7','8','9',0,'J','Q','K'};
	static Uint8 piplocs[10][20]={
		{2,4},
		{2,7,2,1},
		{2,7,2,1,2,4},
		{1,7,3,7,1,1,3,1},
		{1,7,3,7,1,1,3,1,2,4},
		{1,7,3,7,1,1,3,1,1,4,3,4},
		{1,7,3,7,1,1,3,1,1,4,3,4,2,2},
		{1,7,3,7,1,1,3,1,1,4,3,4,2,2,2,6},
		{1,7,1,5,1,3,1,1,3,7,3,5,3,3,3,1,2,4},
		{1,7,1,5,1,3,1,1,3,7,3,5,3,3,3,1,2,2,2,6}
	};
	drawframe(x,y,7,11,crayola[White]);
	drawfilledrect(x+1,y+1,x+5,y+9,' ',0,crayola[White]);//drawrect(x+1,y+1,x+1+5,y+1+9,' ',0,crayola[White]);
	//drawchar(x+1,y+1,0,0,crayola[White]);
	Uint32 pipcolour=(suit>1)?0:crayola[Red];
	Uint8 pipchar=3+suit;
	drawchar(x+1,y+1,rankchar[value],pipcolour,crayola[White]);
	drawchar(x+5,y+9,rankchar[value],pipcolour,crayola[White]);
	drawchar(x+1,y+2,pipchar,pipcolour,crayola[White]);
	drawchar(x+5,y+8,pipchar,pipcolour,crayola[White]);

	if (value>9){	
		drawsprite(x+2,y+2,3,7,((unsigned char*)&courtcards)+((value-10)*7*3*7));
	} else {
		for(Uint8 f=0;f<(value*2)+2;f+=2){
			drawchar(piplocs[value][f]+1+x,piplocs[value][f+1]+1+y,pipchar,pipcolour,crayola[White]);
		}
	}

}
void PangoConsole::flashchar(int x,int y,bool reverse){
	charcell* cc=chargrid+((y*widthchar)+x);
	if(reverse)
		drawchar(x,y,cc->c,crayola[Black],crayola[White],false);
	else
		drawchar(x,y,cc->c,crayola[White],crayola[Black],false);
	
}
void PangoConsole::flashmaus(int x,int y,bool reverse){
	charcell* cc=chargrid+((y*widthchar)+x);
	if(reverse)
		drawchar(x,y,cc->c,crayola[Magenta],crayola[White],false);
	else
		drawchar(x,y,cc->c,crayola[White],crayola[Magenta],false);
	
}
void PangoConsole::redrawchar(int x,int y,bool updatescreen){
	charcell* cc=chargrid+((y*widthchar)+x);
	drawchar(x,y,cc->c,cc->fg,cc->bg,false,updatescreen);
}

void PangoConsole::testdraw(){
	//draw a reference grid of chars and colours on the left side
	/////////////////////////////////////////////////////////////
	Uint8 c=0,cc=0;
	//draw character grid and colour grid
	for(int f=0;f<16;f++){
		for(int g=0;g<16;g++){
			drawchar(g,f,c++,crayola[White]);
			if(cc<133){
				drawchar(g,f+17,' ',crayola[cc],crayola[cc]);
				cc++;
			}
		}
	}
	drawframe(17,0,widthchar-17,heightchar,crayola[White]);
	drawstring(0,28,"cursors: move",crayola[White]);
	drawstring(0,29,"w/LSHFT: charsel",crayola[White]);
	drawstring(0,30,"insert: draw char",crayola[White]);
	drawstring(0,31,"del: clear cell",crayola[White]);
	drawstring(0,32,"w/RSHFT: bg sel",crayola[White]);
	drawstring(0,33,"w/RCTRL: fg sel",crayola[White]);
	drawstring(0,34,"HOME: get cell fg",crayola[BlueBell]);
	drawstring(0,35,"END:  get cell bg",crayola[BlueBell]);
	drawstring(0,36,"PgUp: get char",crayola[BlueBell]);
	drawstring(0,37,"F1: load",crayola[Cerulean]);
	drawstring(0,38,"F2: save",crayola[Cerulean]);
	drawstring(0,39,"F3: export c++",crayola[Cerulean]);
	drawstring(0,40,"F4: POINT",crayola[PinkSherbert]);
	drawstring(0,41,"F5: LINE",crayola[PinkSherbert]);
	drawstring(0,42,"F6: BOCHS",crayola[PinkSherbert]);
	drawstring(0,43,"F7: CIRCA",crayola[PinkSherbert]);
	
	

	

	
	/////////////////////////////////////////////////////////////

	Uint8 function=0; //0 = single char, 1=line
	drawstring(18,0,"POINT",White);
	bool dragging=false;//works for bochs and line
	int dragfromx=0,dragfromy=0;
//	int dragtox=0,dragtoy=0;
	int drawcursorx=18,drawcursory=1;
	bool cursorinverted=false;
	Uint32 cursortick;

	SDL_Event event;
	//current position on character select grid
	int charselectx=0,charselecty=0; Uint8 charselectc=0;
	drawchar(charselectx,charselecty,charselectc,crayola[RedOrange],crayola[Yellow],false);
	//current position on colour select grid, fg and bg.
	int fgselectx=13,fgselecty=24; Uint8 drawcolourfg=White;
	int bgselectx=9,bgselecty=17; Uint8 drawcolourbg=Black;
	drawchar(fgselectx,fgselecty,'F',crayola[Gray],crayola[drawcolourfg]);
	drawchar(bgselectx,bgselecty,'B',crayola[Gray],crayola[drawcolourbg]);

	 SDL_EnableUNICODE(1);

	/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
	/* more events on the event queue, our while loop will exit when */
	/* that occurs.  */
	cursortick=SDL_GetTicks();
	
	OPENFILENAME ofn;
	HANDLE hf;              // file handle

	while(1){
		Uint32 sgt;
		sgt=SDL_GetTicks();
		if(sgt>cursortick+500){
			cursortick=sgt;
			flashchar(drawcursorx,drawcursory,cursorinverted);
			cursorinverted=! cursorinverted;
		}
		while( SDL_PollEvent(&event) ){
	/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
		switch( event.type ){
		case SDL_MOUSEMOTION://maus is moobing
			// event.motion.xrel, event.motion.yrel,event.motion.x, event.motion.y
			
			{
				static int mausx=0,mausy=0;
				static bool virgin=true;
				
				int x=event.motion.x/(fontsizex*zoomx);
				int y=event.motion.y/(fontsizey*zoomy);
				
				
				if(  ((x<16 && y>=17 && y<=24)||(y==25 && x<5)) & !dragging ){ //in colour part
					if(x==bgselectx && y==bgselecty){
						drawcharnobg(x,y,'B',crayola[Gray]);
					} else {
						if(x==fgselectx && y==fgselecty){
							drawcharnobg(x,y,'F',crayola[Gray]);
						} }
							redrawchar(mausx,mausy);
						
					drawcharnobg(x,y,'x',crayola[Black],false);
				} else {
					if(dragging){//remmed newpango
						if(x<=18)x=18;
						if(y==0)y=1;
						if(y==heightchar-1)y=heightchar-2;
						if(x==widthchar-1)x=widthchar-2;
						//if(x>17 && x<widthchar-1 && y>0 && y<heightchar-1){ //newnewnew
						switch(function){
							case 1: //line
								//draw a line to wipe out what was there
								redrawbresline(dragfromx,dragfromy,mausx,mausy);
								//draw the new line in temp form
								bresline(dragfromx,dragfromy,x,y,charselectc,crayola[drawcolourfg],crayola[drawcolourbg],false);
								break;
							case 2://bochs
								redrawrect(dragfromx,dragfromy,mausx,mausy);
								drawrect(dragfromx,dragfromy,x,y,charselectc,crayola[drawcolourfg],crayola[drawcolourbg],false);
								break;
							case 3://circa
								int delta1=abs(mausx-dragfromx),delta2=abs(mausy-dragfromy);
								int radius=(delta1>delta2)?delta1:delta2;
								int delta3=abs(x-dragfromx),delta4=abs(y-dragfromy);
								int radius2=(delta3>delta4)?delta3:delta4;

								if((dragfromy-radius2)<2)radius2=dragfromy-1;
								if((dragfromy-radius)<2)radius=dragfromy-1;

								if((dragfromx-radius2)<19)radius2=dragfromx-18;
								if((dragfromx-radius)<19)radius=dragfromx-18;

								if((dragfromy+radius2)>heightchar-2)radius2=heightchar-dragfromy-2;
								if((dragfromy+radius)>heightchar-2)radius=heightchar-dragfromy-2;

								if((dragfromx+radius2)>widthchar-2)radius2=widthchar-dragfromx-2;
								if((dragfromx+radius)>widthchar-2)radius=widthchar-dragfromx-2;

								//std::cout << radius << "  " << radius2 << std::endl;
								redrawcircle(dragfromx,dragfromy,radius);
								drawcircle(dragfromx,dragfromy,radius2,charselectc,crayola[drawcolourfg],crayola[drawcolourbg],false);
								break;
						}//end switch function
					} else {
						if(!virgin)redrawchar(mausx,mausy);
						flashmaus(x,y);
						virgin=false;
					}
				}
				
				mausx=x;mausy=y;
				
			}
			break;
		case SDL_MOUSEBUTTONUP:// maus booton are "released"
			{
				int x=event.button.x/(fontsizex*zoomx);
				int y=event.button.y/(fontsizey*zoomy);
				if(dragging){
					dragging=false;
					if(x<=18)x=18;
						if(y==0)y=1;
						if(y==heightchar-1)y=heightchar-2;
						if(x==widthchar-1)x=widthchar-2;
					switch(function){
						case 1:
							bresline(dragfromx,dragfromy,x,y,charselectc,crayola[drawcolourfg],crayola[drawcolourbg]);
							break;
						 case 2:
							if(event.button.button==1){
								drawrect(dragfromx,dragfromy,x,y,charselectc,crayola[drawcolourfg],crayola[drawcolourbg]);
							} else {
								drawfilledrect(dragfromx,dragfromy,x,y,charselectc,crayola[drawcolourfg],crayola[drawcolourbg]);
							}
							break;
						case 3://TODO FIX MEH
							//radius is the larger of x-dragfromx and y-dragfromy
							int delta1=abs(x-dragfromx),delta2=abs(y-dragfromy);
							int radius=(delta1>delta2)?delta1:delta2;

							//if dragfromx-radius or dragfromy-radius is out of bounds then don't draw circa
							if((dragfromy-radius)<2)radius=dragfromy-1;
							if((dragfromx-radius)<19)radius=dragfromx-18;
							if((dragfromy+radius)>heightchar-2)radius=heightchar-dragfromy-2;
							if((dragfromx+radius)>widthchar-2)radius=widthchar-dragfromx-2;
							if(event.button.button==1){
								drawcircle(dragfromx,dragfromy,radius,charselectc,crayola[drawcolourfg],crayola[drawcolourbg]);
							} else {
								drawfilledcircle(dragfromx,dragfromy,radius,charselectc,crayola[drawcolourfg],crayola[drawcolourbg]);
							}
						break;
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN://maus booton are "pressed"
			// event.button.button, event.button.x, event.button.y
			{
				int x=event.button.x/(fontsizex*zoomx);
				int y=event.button.y/(fontsizey*zoomy);
				if((x<16 && y>=17 && y<=24)||(y==25 && x<5)){//select colour
					//don't allow maus select colour if this skwaer is the bg or the fg colour
					if( (!(x==bgselectx && y==bgselecty)) && (!(x==fgselectx && y==fgselectx))){
						Uint8 c=((y-17)*16)+x;
						if(event.button.button==1){
							drawcharcharonly(fgselectx,fgselecty,' ');
							fgselectx=x;fgselecty=y;
							drawcolourfg=c;
							drawchar(fgselectx,fgselecty,'F',crayola[Gray],crayola[drawcolourfg]);
						} else {
							drawcharcharonly(bgselectx,bgselecty,' ');
							bgselectx=x;bgselecty=y;
							drawcolourbg=c;
							drawchar(bgselectx,bgselecty,'B',crayola[Gray],crayola[drawcolourbg]);
						}
					}
				}
//experimental
				 else 
					if(x<16 && y<16){ //select character
						drawcharbgfgonly(charselectx,charselecty,crayola[White],crayola[Black]);
						charselectx=x;charselecty=y;
						charselectc=((y)*16)+x;
						drawchar(charselectx,charselecty,charselectc,crayola[RedOrange],crayola[Yellow]);
						drawstring(6,16,std::to_string(charselectc)+"   ",YellowGreen,Black);
					}
				
			//}
//end experimental				
				 else { //drawing and such like
					if(x>17 && x<widthchar-1 && y>0 && y<heightchar-1){
					 if(function>0){//line or bochs	
						 dragging=true;
						 dragfromx=x;
						 dragfromy=y;
					 } else //point
				//	if(x>17 && x<widthchar-1 && y>0 && y<heightchar-1){
						if(event.button.button==1){
							drawchar(x,y,charselectc,crayola[drawcolourfg],crayola[drawcolourbg]);
						} else {
							drawchar(x,y,' ',crayola[Black],crayola[Black]);
						}
					}
				}
			}
			break;
		case SDL_KEYDOWN:
				if( event.key.keysym.unicode < 0x80 && event.key.keysym.unicode > 31 ){
					drawchar(drawcursorx,drawcursory,event.key.keysym.unicode,
						crayola[drawcolourfg],crayola[drawcolourbg]);
						if(drawcursorx<widthchar-2)drawcursorx++; //don't advance cursor if on edge
					}
				switch(event.key.keysym.sym){
				case SDLK_F4: //point (what it starts on)
					if(function!=0){
						function=0;
						drawstring(18,0,"POINT",White);
					}
					break;
				case SDLK_F5: //line
					if(function!=1){
						function=1;
						drawstring(18,0,"LINE ",White);
					}
					break;
				case SDLK_F6: //bochs
					if(function!=2){
						function=2;
						drawstring(18,0,"BOCHS",White);
					}
					break;
				case SDLK_F7: //circa
					if(function!=3){
						function=3;
						drawstring(18,0,"CIRCA",White);
					}
					break;
				case SDLK_F1://load
		
					wchar_t szFile[260];       // buffer for file name
					//HWND hwnd;              // owner window
					

					// Initialize OPENFILENAME
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = winhandle;
					ofn.lpstrFile = szFile;
					// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
					// use the contents of szFile to initialize itself.
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					// Display the Open dialog box. 

					if (GetOpenFileName(&ofn)==TRUE) {
						std::ifstream ifile;
						char block[7];
						ifile.open(ofn.lpstrFile,std::ios::in | std::ios::binary);
						for(int g=1;g<heightchar-1;g++){
							for(int f=18;f<widthchar-1;f++){
								charcell* cc=chargrid+((g*widthchar)+f);
								ifile.read(block,7);
								cc->bg=SDL_MapRGB(screen->format,block[0],block[1],block[2]);
								cc->fg=SDL_MapRGB(screen->format,block[3],block[4],block[5]);
								cc->c=block[6];
								redrawchar(f,g);
								
							}
						}
						ifile.close();
					}				
					break;
				case SDLK_F2://save
					ZeroMemory(&ofn, sizeof(ofn));
					szFile[0] = '\0'; 
					ofn.lStructSize = sizeof(OPENFILENAME); 
					ofn.hwndOwner = winhandle; 
					ofn.lpstrFilter =  L"All\0*.*\0Text\0*.TXT\0";
					ofn.lpstrFile= szFile; 
					ofn.nMaxFile = sizeof(szFile)/ sizeof(*szFile); 
					ofn.lpstrFileTitle = NULL; //the title of the file
					ofn.nMaxFileTitle = NULL; //length of above
					ofn.lpstrInitialDir = NULL; 
					ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT; 
					ofn.lpstrTitle = NULL;  //will be "save as" if null
 
					if (GetSaveFileName(&ofn)==TRUE){
						std::ofstream ofile;
						ofile.open(ofn.lpstrFile,std::ios::out | std::ios::binary);
						for(int g=1;g<heightchar-1;g++){
							for(int f=18;f<widthchar-1;f++){
								charcell* cc=chargrid+((g*widthchar)+f);
								Uint8 red,green,blue;
								SDL_GetRGB(cc->bg,screen->format,&red,&green,&blue);
								ofile << red << green << blue ;
								SDL_GetRGB(cc->fg,screen->format,&red,&green,&blue);
								ofile << red << green << blue << cc->c ;

							}
						}
						ofile.close();
					}
					break;
				case SDLK_F3: //export as c++
					ZeroMemory(&ofn, sizeof(ofn));
					szFile[0] = '\0'; 
					ofn.lStructSize = sizeof(OPENFILENAME); 
					ofn.hwndOwner = winhandle; 
					ofn.lpstrFilter =  L"All\0*.*\0Text\0*.TXT\0";
					ofn.lpstrFile= szFile; 
					ofn.nMaxFile = sizeof(szFile)/ sizeof(*szFile); 
					ofn.lpstrFileTitle = NULL; //the title of the file
					ofn.nMaxFileTitle = NULL; //length of above
					ofn.lpstrInitialDir = NULL; 
					ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT; 
					ofn.lpstrTitle =L"Export as:"; 
 
					if (GetSaveFileName(&ofn)==TRUE){
						//work out the smallest rectangle that fits all the gfx in

						int bottom=1,right=18;
						for(int f=18;f<widthchar-1;f++){
							for(int g=bottom;g<heightchar-1;g++){
								charcell* cg=chargrid+((g*widthchar)+f);
								if(cg->bg!=cg->fg)bottom=g;
							}
						}
						for(int g=1;g<heightchar-1;g++){
							for(int f=right;f<widthchar-1;f++){
								charcell* cg=chargrid+((g*widthchar)+f);
								if(cg->bg!=cg->fg)right=f;
							}
						}

						std::ofstream ofile;
						ofile.open(ofn.lpstrFile,std::ios::out);
						ofile << "const unsigned char mygfx[" << bottom << "][" << right-17
							<< "][7] = {" << std::endl;
						Uint8 linemarker=0;

						for(int g=1;g<bottom+1;g++){
							for(int f=18;f<right+1;f++){
								ofile << "{";
								charcell* cc=chargrid+((g*widthchar)+f);
								Uint8 red,green,blue;
								SDL_GetRGB(cc->bg,screen->format,&red,&green,&blue);
								ofile << (int)red << "," << (int)green << "," << (int)blue << ",";
								SDL_GetRGB(cc->fg,screen->format,&red,&green,&blue);
								ofile << (int)red << "," << (int)green << "," << (int)blue << "," << (int)(cc->c) << "}";
								if(!(g==bottom&&f==right)){
									ofile << "," ;
									if(++linemarker==4){
										linemarker=0;
										ofile << std::endl;
									}
								}
							}
						}
						ofile << "}" << std::endl;
						ofile.close();
					}
					break;
				case SDLK_DOWN:////////////////////////////////////////////////////////////
					if(event.key.keysym.mod&KMOD_RCTRL){
						if(fgselecty<24||(fgselecty==24 && fgselectx<5)){
							if((fgselectx!=bgselectx)||(fgselecty+1!=bgselecty)){
								drawcharcharonly(fgselectx,fgselecty,' ');
								fgselecty++;drawcolourfg+=16;
								drawchar(fgselectx,fgselecty,'F',crayola[Gray],crayola[drawcolourfg]);
							}
						}
					} else
					if(event.key.keysym.mod&KMOD_RSHIFT){
						if(bgselecty<24||(bgselecty==24 && bgselectx<5)){
							if((fgselectx!=bgselectx)||(bgselecty+1!=fgselecty)){	
								drawcharcharonly(bgselectx,bgselecty,' ');
								bgselecty++;drawcolourbg+=16;
								drawchar(bgselectx,bgselecty,'B',crayola[Gray],crayola[drawcolourbg]);
							}
						}
					} else
					if(event.key.keysym.mod&KMOD_LSHIFT){
						if(charselecty<15){
							drawcharbgfgonly(charselectx,charselecty,crayola[White],crayola[Black]);
							charselecty++;charselectc+=16;
							drawchar(charselectx,charselecty,charselectc,crayola[RedOrange],crayola[Yellow]);
						}
					} else
					if(drawcursory<heightchar-2){
						redrawchar(drawcursorx,drawcursory);
						drawcursory++;
					}
					break;
				case SDLK_UP://///////////////////////////////////////////////////////////////
					if(event.key.keysym.mod&KMOD_RCTRL){
						if(fgselecty>17){
							if((fgselectx!=bgselectx)||(fgselecty-1!=bgselecty)){
								drawcharcharonly(fgselectx,fgselecty,' ');
								fgselecty--;drawcolourfg-=16;
								drawchar(fgselectx,fgselecty,'F',crayola[Gray],crayola[drawcolourfg]);
							}
						}
					} else
					if(event.key.keysym.mod&KMOD_RSHIFT){
						if(bgselecty>17){
							if((fgselectx!=bgselectx)||(bgselecty-1!=fgselecty)){
								drawcharcharonly(bgselectx,bgselecty,' ');
								bgselecty--;drawcolourbg-=16;
								drawchar(bgselectx,bgselecty,'B',crayola[Gray],crayola[drawcolourbg]);
							}
						}
					} else
					if(event.key.keysym.mod&KMOD_LSHIFT){
						if(charselecty>0){
							drawcharbgfgonly(charselectx,charselecty,crayola[White],crayola[Black]);
							charselecty--;charselectc-=16;
							drawchar(charselectx,charselecty,charselectc,crayola[RedOrange],crayola[Yellow]);
						}
					} else
					if(drawcursory>1){
						redrawchar(drawcursorx,drawcursory);
						drawcursory--;
					}
					break;
					
				case SDLK_RIGHT://////////////////////////////////////////////////////////////////
					if(event.key.keysym.mod&KMOD_RCTRL){
						if((fgselecty<25 && fgselectx<15)||(fgselecty==25 && fgselectx<4)){
							if((fgselecty!=bgselecty)||(fgselectx+1!=bgselectx)){
								drawcharcharonly(fgselectx,fgselecty,' ');
								fgselectx++;drawcolourfg++;
								drawchar(fgselectx,fgselecty,'F',crayola[Gray],crayola[drawcolourfg]);
							}
						}
					} else
					if(event.key.keysym.mod&KMOD_RSHIFT){
						if((bgselecty<25 && bgselectx<15)||(bgselecty==25 && bgselectx<4)){
							if((fgselecty!=bgselecty)||(bgselectx+1!=fgselectx)){
								drawcharcharonly(bgselectx,bgselecty,' ');
								bgselectx++;drawcolourbg++;
								drawchar(bgselectx,bgselecty,'B',crayola[Gray],crayola[drawcolourbg]);
							}
						}
					} else
					if(event.key.keysym.mod&KMOD_LSHIFT){
						if(charselectx<15){
							drawcharbgfgonly(charselectx,charselecty,crayola[White],crayola[Black]);
							charselectx++;charselectc++;
							drawchar(charselectx,charselecty,charselectc,crayola[RedOrange],crayola[Yellow]);
						}
					} else
					if(drawcursorx<widthchar-2){
						redrawchar(drawcursorx,drawcursory);
						drawcursorx++;
					}
					break;
				case SDLK_LEFT:////////////////////////////////////////////////////////////////////////////
					if(event.key.keysym.mod&KMOD_RCTRL){
						if(fgselectx>0){
							if((fgselecty!=bgselecty)||(fgselectx-1!=bgselectx)){
								drawcharcharonly(fgselectx,fgselecty,' ');
								fgselectx--;drawcolourfg--;
								drawchar(fgselectx,fgselecty,'F',crayola[Gray],crayola[drawcolourfg]);
							}
						}
					} else
					if(event.key.keysym.mod&KMOD_RSHIFT){
						if(bgselectx>0){
							if((fgselecty!=bgselecty)||(bgselectx-1!=fgselectx)){
								drawcharcharonly(bgselectx,bgselecty,' ');
								bgselectx--;drawcolourbg--;
								drawchar(bgselectx,bgselecty,'B',crayola[Gray],crayola[drawcolourbg]);
							}
						}
					} else
					if(event.key.keysym.mod&KMOD_LSHIFT){
						if(charselectx>0){
							drawcharbgfgonly(charselectx,charselecty,crayola[White],crayola[Black]);
							charselectx--;charselectc--;
							drawchar(charselectx,charselecty,charselectc,crayola[RedOrange],crayola[Yellow]);
						}
					} else
					if(drawcursorx>18){
						redrawchar(drawcursorx,drawcursory);
						drawcursorx--;
					}
					break;
				case SDLK_RETURN:
					if(drawcursory<heightchar-2){
						redrawchar(drawcursorx,drawcursory);
						drawcursory++;
						drawcursorx=18;
					}
					break;
				case SDLK_INSERT://put char from char select screen into cell
					drawchar(drawcursorx,drawcursory,charselectc,crayola[drawcolourfg],crayola[drawcolourbg]);
					if(drawcursorx<widthchar-2)drawcursorx++; //don't advance cursor if on edge
					break;
				case SDLK_DELETE://clear cell
					drawchar(drawcursorx,drawcursory,' ',crayola[Black],crayola[Black]);
					if(drawcursorx<widthchar-2)drawcursorx++; //don't advance cursor if on edge
					break;
				case SDLK_PAGEUP: //grab cell character
					{
						charcell* cc=chargrid+((drawcursory*widthchar)+drawcursorx);
						redrawchar(charselectx,charselecty);
						charselectc=cc->c;
						charselecty=charselectc/16;
						charselectx=charselectc%16;
						drawchar(charselectx,charselecty,charselectc,crayola[RedOrange],crayola[Yellow],false);
					}
					break;
				case SDLK_HOME://pick up foreground colour
					{
						charcell* cc=chargrid+((drawcursory*widthchar)+drawcursorx);
						Uint32 fg=cc->fg;
						if(fg!=crayola[drawcolourbg]){//if fg would be what bg is then skip
							for(Uint8 f=0;f<133;f++){
								if(crayola[f]==fg){
									drawcolourfg=f;
									redrawchar(fgselectx,fgselecty);
									fgselecty=(f/16)+17;
									fgselectx=f%16;
									drawchar(fgselectx,fgselecty,'F',crayola[Gray],crayola[drawcolourfg],false);
								}
							}
						}
					}
					break;
				case SDLK_END://pick up background colour
					{
						charcell* cc=chargrid+((drawcursory*widthchar)+drawcursorx);
						Uint32 bg=cc->bg;
						if(bg!=crayola[drawcolourfg]){//if fg would be what bg is then skip
							for(Uint8 f=0;f<133;f++){
								if(crayola[f]==bg){
									drawcolourbg=f;
									redrawchar(bgselectx,bgselecty);
									bgselecty=(f/16)+17;
									bgselectx=f%16;
									drawchar(bgselectx,bgselecty,'B',crayola[Gray],crayola[drawcolourbg],false);
								}
							}
						}
					}
					break;
				case SDLK_BACKSPACE:
					if(drawcursorx>18){
						redrawchar(drawcursorx,drawcursory);
						drawcursorx--;
						drawchar(drawcursorx,drawcursory,' ',crayola[drawcolourfg],crayola[drawcolourbg]);
					}
					break;
				}
			break;

			case SDL_KEYUP:
			//printf( "Key release detected\n" );
			break;

			default:
			break;
		}
	}
}



}

void PangoConsole::drawsprite(int x,int y,int w,int h,unsigned char* array){
	if(x<0 || x>widthchar-w || y<0 || y>heightchar-h){
		showerror(winhandle,L"sprite off screen");
	}

	unsigned char* marker=array;
	Uint8 r,g,b;
	for(int gg=0;gg<h;gg++){
		for(int ff=0;ff<w;ff++){
			r=*array++;g=*array++;b=*array++;
			Uint32 bg=SDL_MapRGB(screen->format,r,g,b);
			r=*array++;g=*array++;b=*array++;
			Uint32 fg=SDL_MapRGB(screen->format,r,g,b);
			drawchar(x+ff,y+gg,*array++,fg,bg);
		}
	}

}

void PangoConsole::colourgrid(int x,int y){
	static Uint8 rg[8]={0,51,92,127,162,193,225,255};
	static Uint8 b[4]={0,104,183,255};
	for(int f=0;f<256;f++){
		Uint8 rr=rg[f>>5];
		Uint8 gg=rg[(f&28)>>2];
		Uint8 bb=b[f&3];
		Uint32 c=SDL_MapRGB(screen->format,rr,gg,bb);
		drawchar(x+(f%16),y+(f/16),' ',c,c);
	}
	
}

void PangoConsole::bresline(int x0,int y0,int x1,int y1,Uint8 c,Uint32 fg,Uint32 bg,bool update){
	 
   int dx = abs(x1-x0);
   int dy = abs(y1-y0); 
   int sx=(x0 < x1)?1:-1;
   int sy=(y0 < y1)?1:-1;
   int err= dx-dy;
 
   while(1){
		drawchar(x0,y0,c,fg,bg,update);
		if(x0 == x1 && y0 == y1)break;
		int e2 = 2*err;
		if (e2 > -dy){
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx){
			err += dx;
			y0 += sy;
		}
   }
}

void PangoConsole::redrawbresline(int x0,int y0,int x1,int y1){
	 
   int dx = abs(x1-x0);
   int dy = abs(y1-y0); 
   int sx=(x0 < x1)?1:-1;
   int sy=(y0 < y1)?1:-1;
   int err= dx-dy;
 
   while(1){
		redrawchar(x0,y0);
		if(x0 == x1 && y0 == y1)break;
		int e2 = 2*err;
		if (e2 > -dy){
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx){
			err += dx;
			y0 += sy;
		}
   }
}


void PangoConsole::FlashActiveCursor(){
	if(!(CurrentlyActiveArea->ShowCursor))return;
	if(!(CurrentlyActiveArea->CursorIsVisible()))return;
	static Uint32 cursortick=0;
	static bool cursorinverted=false;
	Uint32 sgt=SDL_GetTicks();
	if(sgt>cursortick+500){
		cursortick=sgt;
		CurrentlyActiveArea->FlashCursor(cursorinverted);
		cursorinverted=!cursorinverted;
	}
}