#include "filterdefault.hpp"

namespace flounder
{
	filterdefault::filterdefault() :
		ipostfilter("filterDefault", "res/shaders/filters/default.frag.spv", nullptr)
	{
	}

	filterdefault::~filterdefault()
	{
	}

	void filterdefault::storeValues()
	{
	}
}
