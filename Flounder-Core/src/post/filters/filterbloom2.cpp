#include "filterbloom2.hpp"

namespace flounder
{
	filterbloom2::filterbloom2() :
		ipostfilter("filterBloom2", "res/shaders/filters/bloom2.frag.spv")
	{
	}

	filterbloom2::~filterbloom2()
	{
	}

	void filterbloom2::storeValues()
	{
	}
}
