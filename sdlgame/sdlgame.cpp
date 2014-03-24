// sdlgame.cpp : Defines the entry point for the console application.
//




#include <tchar.h>

#include "sdl.h"

#include "playingcards.h"




#include "pangoconsole.h"
#include "colours.h"
#include "error.h"

#include <random>
#include <time.h>

#include "area.h"

int _tmain(int argc, _TCHAR* argv[]){

	PangoConsole console(1280,720,(unsigned char*)amstrad437,8,8,2,2);//amstrad437 font, windowed
	//PangoConsole console(1280,720,(unsigned char*)notofont,16,16,1,1);
	//PangoConsole console(1920,1080,(unsigned char*)amstrad437,8,8,3,3,true);//8,8,2,2 for amstrad font, 16,16,1,1 for libtcod
	//PangoConsole console(1280,720,(unsigned char*)amsfont,8,8,2,2);//8,8,2,2 for amstrad font, 16,16,1,1 for libtcod
	//PangoConsole console(640,720,(unsigned char*)amstrad437,8,8,1,2,false);
	std::default_random_engine e;
	e.seed(time(0));

	console.testdraw();
	/*pile masobberettisl(true,true);
	Uint8 brb=masobberettisl.onefromthetop();
	Uint8 brb2=masobberettisl.onefromthetop();
	Uint8 brb3=masobberettisl.onefromthetop();
	Uint8 brb4=masobberettisl.onefromthetop();
	Uint8 brb5=masobberettisl.onefromthetop();

	console.drawcard(30,30,brb%13,brb/13);
	console.drawcard(40,30,brb2%13,brb2/13);
	console.drawcard(50,30,brb3%13,brb3/13);
	console.drawcard(60,30,brb4%13,brb4/13);
	console.drawcard(70,30,brb5%13,brb5/13);

	masobberettisl.putbackbottom(brb);*/
	//console.drawchar(1,1,'e',crayola[White]);

	console.drawframe(0,33,80,12,crayola[White]);
	//78 wide, 20 high, at screen pos: 1,36, and on screen width 78, on-screen height 10
	Area messagelog(78,20,1,34,78,10,console);
	messagelog.ShowCursor=true;

	messagelog.PrintString("void Area::DrawChar(int x,int y,unsigned char c,Uint32 fg,Uint32 bg,bool updategrid=true,bool updatescreen=true);");
	messagelog.PrintString("MOTHERFUCKER THIS IS NEXT!",Red);
	messagelog.PrintString("123bucko\n");
	messagelog.PrintString("finally this!",RedOrange);
	//messagelog.DrawString(0,0,"Hello, world!");
	//messagelog.DrawString(0,1,"Welcome to pangolin kwest 1.0 lol ya right.");
	//messagelog.DrawString(0,2,"The orc woman sucks your dick for 5 sex damage");
	//messagelog.DrawString(0,3,"You counter with breast bump for 10 sex damage");
	//messagelog.DrawString(0,4,"not much happens");
	//messagelog.DrawString(0,5,"Now I'm going to leave 5 lines blank");
	//messagelog.DrawString(0,11,"and we're back");
	//messagelog.DrawString(0,12,"This is line 13, code name line 12");
	//messagelog.DrawString(0,19,"last line");
	//for(auto f=0;f<20;++f){
		//for(auto ff=0;ff<78;ff++){
		//	messagelog.drawchar(ff,f,ff,crayola[f]);
	//	}
		//messagelog.drawchar(0,f,f+48,crayola[f]);
	//}

	//messagelog.SetOrigin(0,10);


//best way to do is create your pangoconsole, this is the screen
//then you create a message log window, width this, height that, pos this and that on the screen
//with a link to the screen

//then you create the dungeonarea on the screen same params

//then you're playing the game, you say print to main screen (a default area of the whole screen)
		//or you print to the message log, or you print to the dungeon area



	/*for(int f=0;f<10;f++){
		console.bresline(e()%80,e()%45,e()%80,e()%45,e()%256,crayola[e()%133],crayola[e()%133]);
	}

	console.colourgrid(30,10);*/
	
	

	//for(int f=0;f<10;f++){
	//	console.DrawWuLine(e()%80,e()%45,e()%80,e()%45,e()%256,e()%256,e()%256);
	//}

	//for(int f=0;f<13;f++){
	//	console.drawcard(f*8,0,f,0);
	//	console.drawcard(f*8,14,f,1);
	//	console.drawcard(f*8,28,f,2);
	//	console.drawcard(f*8,42,f,3);
	//}




	//showerror(L"tenhi was not here");
	//console.drawstring(0,55,"SNAIL BRAND PLAYING CARDS OMG by graspee. Hello #RGRD!",White);
	//console.drawstring(0,1,"_@y    snail",White);
	/*console.drawstring(0,0,"This sword is an example of the kind of",Red);
	console.drawstring(0,1,"thing you might want to hit an orc with",White);
	console.drawstring(0,2,"HP: 212   MP: 199   SP: 186    LV-E@2!!",ForestGreen);
	console.drawstring(0,3,"Merry Christmas to all our fans!",RoyalPurple);*/

	for(;;){
		SDL_WaitEvent(0);
		console.FlashActiveCursor();
	
	}
	return 0;
}

