#ifndef _playingcards_h
#define _playingcards_h

#include <vector>
#include "sdl.h"

#define __nocards 99

 const unsigned char courtcards[3][7][3][7] = {
	{
		{{255,255,255,222,170,136,32},{255,255,255,188,93,88,227},{255,255,255,188,93,88,32}},
		{{255,255,255,176,183,198,32},{255,255,255,239,152,170,1},{255,255,255,188,93,88,32}},
		{{255,255,255,28,172,120,214},{255,255,255,0,0,0,20},{255,255,255,28,172,120,215}},
		{{28,172,120,253,188,180,229},{28,172,120,255,117,56,32},{28,172,120,253,188,180,229}},
		{{116,66,200,102,255,102,142},{116,66,200,102,255,102,157},{116,66,200,102,255,102,142}},
		{{116,66,200,102,255,102,142},{255,255,255,26,72,118,32},{116,66,200,102,255,102,142}},
		{{255,255,255,26,72,118,137},{255,255,255,31,117,254,32},{255,255,255,26,72,118,137}},
	},
	{
		{{255,255,255,222,170,136,32},{255,255,255,195,100,197,234},{255,255,255,222,170,136,32}},
		{{255,255,255,31,117,254,167},{255,255,255,253,221,230,1},{255,255,255,31,117,254,131}},
		{{255,255,255,31,117,254,178},{238,32,77,255,255,0,134},{255,255,255,31,117,254,163}},
		{{238,32,77,253,221,230,34},{238,32,77,255,255,0,183},{238,32,77,253,221,230,34}},
		{{238,32,77,254,78,218,181},{238,32,77,254,78,218,181},{238,32,77,254,78,218,181}},
		{{238,32,77,254,78,218,182},{238,32,77,254,78,218,182},{238,32,77,254,78,218,182}},
		{{255,255,255,31,206,203,215},{255,255,255,31,117,254,32},{255,255,255,31,206,203,215}},

	},
	{
		{{255,255,255,222,170,136,32},{255,255,255,255,174,66,236},{255,255,255,151,154,170,179}},
		{{255,255,255,176,183,198,32},{255,255,255,255,188,217,2},{255,255,255,151,154,170,179}},
		{{255,255,255,176,183,198,232},{255,255,255,176,183,198,178},{255,255,255,151,154,170,179}},
		{{255,255,255,176,183,198,168},{255,255,255,176,183,198,15},{255,255,255,151,154,170,136}},
		{{255,255,255,176,183,198,178},{255,255,255,176,183,198,178},{255,255,255,176,183,198,178}},
		{{255,255,255,176,183,198,178},{255,255,255,176,183,198,178},{255,255,255,176,183,198,178}},
		{{255,255,255,176,183,198,13},{255,255,255,31,117,254,32},{255,255,255,176,183,198,13}}

	}
};
// struct card {
//	 Uint8 value;
//	 Uint8 suit;
//};

 class pile {
	 std::vector <Uint8> cards; //store 0-51. use / and % to get value and suit
 public:
	 pile(bool fulldeck=true,bool shuffle=true);
	 //~pile();
	 void shuffle();
	 Uint8 peektop();
	 Uint8 onefromthetop();
	 Uint8 onefromthebottom();
	 void putbackbottom(Uint8 r);
	 void filldeck();
 };


#endif