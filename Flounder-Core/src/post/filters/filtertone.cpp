#include "filtertone.h"

namespace flounder
{
	filtertone::filtertone() :
		ipostfilter("filterTone", "res/shaders/filters/toneFragment.glsl")
	{
	}

	filtertone::~filtertone()
	{
	}

	void filtertone::storeValues()
	{
	}
}
