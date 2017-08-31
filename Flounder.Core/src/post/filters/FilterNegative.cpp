#include "filternegative.hpp"

namespace Flounder
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
