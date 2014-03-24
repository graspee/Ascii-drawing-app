#ifndef _colours_h_
#define _colours_h_

#include "sdl.h"

enum Ecrayola { Almond, AntiqueBrass, Apricot, Aquamarine, Asparagus, AtomicTangerine, BananaMania, Beaver, Bittersweet, Black,
	BlizzardBlue, Blue, BlueBell, BlueGray, BlueGreen, BlueViolet, Blush, BrickRed, Brown, BurntOrange, BurntSienna, CadetBlue,
	Canary, CaribbeanGreen, CarnationPink, Cerise, Cerulean, Chestnut, Copper, Cornflower, CottonCandy, Dandelion, Denim, DesertSand,
	Eggplant, ElectricLime, Fern, ForestGreen, Fuchsia, FuzzyWuzzy, Gold, Goldenrod, GrannySmithApple, Gray, Green, GreenBlue,
	GreenYellow, HotMagenta, Inchworm, Indigo, JazzberryJam, JungleGreen, LaserLemon, Lavender, LemonYellow, MacaroniandCheese, 
	Magenta, MagicMint, Mahogany, Maize, Manatee, MangoTango, Maroon, Mauvelous, Melon, MidnightBlue, MountainMeadow, Mulberry,
	NavyBlue, NeonCarrot, OliveGreen, Orange, OrangeRed, OrangeYellow, Orchid, OuterSpace, OutrageousOrange, PacificBlue, Peach, 
	Periwinkle, PiggyPink, PineGreen, PinkFlamingo, PinkSherbert, Plum, PurpleHeart, PurpleMountainsMajesty, PurplePizzazz,
	RadicalRed, RawSienna, RawUmber, RazzleDazzleRose, Razzmatazz, Red, RedOrange, RedViolet, RobinsEggBlue, RoyalPurple, Salmon,
	Scarlet, ScreaminGreen, SeaGreen, Sepia, Shadow, Shamrock, ShockingPink, Silver, SkyBlue, SpringGreen, Sunglow, SunsetOrange,
	Tan, TealBlue, Thistle, TickleMePink, Timberwolf, TropicalRainForest, Tumbleweed, TurquoiseBlue, UnmellowYellow, VioletPurple, 
	VioletBlue, VioletRed, VividTangerine, VividViolet, White, WildBlueYonder, WildStrawberry, WildWatermelon, Wisteria, Yellow,
	YellowGreen, YellowOrange, DEFAULTCOLOUR };

extern const unsigned char crayolargb[133][3];



extern Uint32 crayola[133];

void SetupColours(SDL_Surface* screen);
	

#endif
