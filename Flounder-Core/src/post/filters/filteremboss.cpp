#include "filteremboss.h"

namespace flounder 
{
	filteremboss::filteremboss() :
		ipostfilter("filterEmboss", "res/shaders/filters/embossFragment.glsl")
	{
	}

	filteremboss::~filteremboss()
	{
	}

	void filteremboss::storeValues()
	{
	}
}
