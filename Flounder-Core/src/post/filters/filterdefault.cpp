#include "filterdefault.h"

namespace flounder
{
	filterdefault::filterdefault() :
#ifdef FLOUNDER_API_WEB
		ipostfilter("filterDefault", "res/shaders/filters/defaultFragment.web.glsl", NULL)
#else
		ipostfilter("filterDefault", "res/shaders/filters/defaultFragment.glsl", NULL)
#endif
	{
	}

	filterdefault::~filterdefault()
	{
	}

	void filterdefault::storeValues()
	{
	}
}
