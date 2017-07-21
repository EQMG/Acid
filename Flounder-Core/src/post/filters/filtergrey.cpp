#include "filtergrey.h"

namespace flounder
{
	filtergrey::filtergrey() :
		ipostfilter("filterGrey", "res/shaders/filters/greyFragment.glsl")
	{
	}

	filtergrey::~filtergrey()
	{
	}

	void filtergrey::storeValues()
	{
	}
}
