#include "filterdefault.hpp"

namespace Flounder
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
