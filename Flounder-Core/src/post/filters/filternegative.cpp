#include "filternegative.h"

namespace flounder 
{
	filternegative::filternegative() :
		ipostfilter("filterNegative", "res/shaders/filters/negativeFragment.glsl")
	{
	}

	filternegative::~filternegative()
	{
	}

	void filternegative::storeValues()
	{
	}
}
