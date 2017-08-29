#include "filternegative.hpp"

namespace flounder
{
	filternegative::filternegative() :
		ipostfilter("filterNegative", "res/shaders/filters/negative.frag.spv")
	{
	}

	filternegative::~filternegative()
	{
	}

	void filternegative::storeValues()
	{
	}
}
