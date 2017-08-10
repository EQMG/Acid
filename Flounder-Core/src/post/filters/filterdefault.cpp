#include "filterdefault.h"

namespace flounder
{
	filterdefault::filterdefault() :
		ipostfilter("filterDefault", "res/shaders/filters/defaultFragment.glsl", nullptr)
	{
	}

	filterdefault::~filterdefault()
	{
	}

	void filterdefault::storeValues()
	{
	}
}
