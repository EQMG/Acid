#include "filtergrey.h"

namespace flounder {
	filtergrey::filtergrey() :
		ipostfilter("filterGrey", "Resources/shaders/filters/greyFragment.glsl")
	{
	}

	filtergrey::~filtergrey()
	{
	}

	void filtergrey::storeValues()
	{
	}
}
