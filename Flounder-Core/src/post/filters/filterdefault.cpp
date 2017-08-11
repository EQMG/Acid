#include "filterdefault.h"

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
