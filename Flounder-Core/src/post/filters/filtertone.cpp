#include "filtertone.hpp"

namespace Flounder
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
