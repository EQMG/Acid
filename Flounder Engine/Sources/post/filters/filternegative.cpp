#include "filternegative.h"

namespace flounder {
	filternegative::filternegative() :
		ipostfilter("filterNegative", "Resources/shaders/filters/negativeFragment.glsl")
	{
	}

	filternegative::~filternegative()
	{
	}

	void filternegative::storeValues()
	{
	}
}
