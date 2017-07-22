#include "filtersepia.h"

namespace flounder
{
	filtersepia::filtersepia() :
		ipostfilter("filterSepia", "res/shaders/filters/sepiaFragment.glsl")
	{
	}

	filtersepia::~filtersepia()
	{
	}

	void filtersepia::storeValues()
	{
	}
}
