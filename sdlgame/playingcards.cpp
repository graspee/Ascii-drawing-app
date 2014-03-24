#include "playingcards.h"
#include <algorithm>
#include <random>
#include <time.h>

pile::pile(bool fulldeck,bool shuffle){
	if(fulldeck){
		filldeck();
		if(shuffle){
			this->shuffle();
		}
	}
}

//pile::~pile(){
//}

void pile::filldeck(){
		for(int f=0;f<52;f++){
		cards.push_back(f);
	}
}

void pile::shuffle(){
	static std::default_random_engine e;
	e.seed(time(0));
	
	std::shuffle(cards.begin(),cards.end(),e);
}

Uint8 pile::peektop(){
	return(cards.front());
}

Uint8 pile::onefromthetop(){
	if(cards.empty()){
		return __nocards;
	} else {
		Uint8 r=cards.front();
		cards.erase(cards.begin());
		return r;
	}
}

Uint8 pile::onefromthebottom(){
	if(cards.empty()){
		return __nocards;
	} else {
		Uint8 r=cards.back();
		cards.pop_back();
		return r;
	}
}

void pile::putbackbottom(Uint8 r){
	cards.push_back(r);
}