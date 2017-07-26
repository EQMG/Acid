#include "fonts.h"

namespace flounder
{
	fonttype *const fonts::ARIAL = new fonttype("res/fonts/arial.png", "res/fonts/arial.fnt");
	fonttype *const fonts::BERLIN_SANS = new fonttype("res/fonts/berlinSans.png", "res/fonts/berlinSans.fnt");
	fonttype *const fonts::CAFE_FRANCOISE = new fonttype("res/fonts/cafefrancoise.png", "res/fonts/cafefrancoise.fnt");
	fonttype *const fonts::CANDARA = new fonttype("res/fonts/candara.png", "res/fonts/candara.fnt");
	fonttype *const fonts::SEGOE = new fonttype("res/fonts/segoe.png", "res/fonts/segoe.fnt");

	fonts::fonts() : 
		imodule()
	{
	}

	fonts::~fonts()
	{
	}

	void fonts::init()
	{
	}

	void fonts::update()
	{
	}
}
