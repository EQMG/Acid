#include "filtertone.h"

namespace flounder
{
	filtertone::filtertone() :
		ipostfilter("filterTone", "res/shaders/filters/tone.frag.spv")
	{
	}

	filtertone::~filtertone()
	{
	}

	void filtertone::storeValues()
	{
	}
}
