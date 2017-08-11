#include "filteremboss.h"

namespace flounder
{
	filteremboss::filteremboss() :
		ipostfilter("filterEmboss", "res/shaders/filters/emboss.frag.spv")
	{
	}

	filteremboss::~filteremboss()
	{
	}

	void filteremboss::storeValues()
	{
	}
}
