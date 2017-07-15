#include "filteremboss.h"

namespace flounder {
	filteremboss::filteremboss() :
		ipostfilter("filterEmboss", "Resources/shaders/filters/embossFragment.glsl")
	{
	}

	filteremboss::~filteremboss()
	{
	}

	void filteremboss::storeValues()
	{
	}
}
