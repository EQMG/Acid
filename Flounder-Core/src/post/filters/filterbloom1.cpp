#include "filterbloom1.hpp"

namespace flounder
{
	filterbloom1::filterbloom1() :
		ipostfilter("filterBloom1", "res/shaders/filters/bloom1.frag.spv")
	{
	}

	filterbloom1::~filterbloom1()
	{
	}

	void filterbloom1::storeValues()
	{
	}
}
