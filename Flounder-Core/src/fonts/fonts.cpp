#include "fonts.h"

namespace flounder
{
	fonts::fonts() : 
		imodule()
	{
	}

	fonts::~fonts()
	{
		delete arial;
		delete berlinSans;
		delete cafeFrancoise;
		delete candara;
		delete segoe;
	}

	void fonts::init()
	{
		arial = new fonttype("res/fonts/arial.png", "res/fonts/arial.fnt");
		berlinSans = new fonttype("res/fonts/berlinSans.png", "res/fonts/berlinSans.fnt");
		cafeFrancoise = new fonttype("res/fonts/cafefrancoise.png", "res/fonts/cafefrancoise.fnt");
		candara = new fonttype("res/fonts/candara.png", "res/fonts/candara.fnt");
		segoe = new fonttype("res/fonts/segoe.png", "res/fonts/segoe.fnt");
	}

	void fonts::update()
	{
	}
}
