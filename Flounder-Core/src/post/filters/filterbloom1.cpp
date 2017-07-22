#include "filterbloom1.h"

namespace flounder
{
	filterbloom1::filterbloom1() :
		ipostfilter("filterBloom1", "res/shaders/filters/bloom1Fragment.glsl")
	{
	}

	filterbloom1::~filterbloom1()
	{
	}

	void filterbloom1::storeValues()
	{
	}
}
