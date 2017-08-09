#include "filterdefault.h"

namespace flounder
{
	filterdefault::filterdefault() :
		ipostfilter("filterDefault", "res/shaders/filters/defaultFragment.glsl", NULL)
	{
	}

	filterdefault::~filterdefault()
	{
	}

	void filterdefault::storeValues()
	{
	}
}
