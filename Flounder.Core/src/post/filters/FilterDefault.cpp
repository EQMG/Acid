#include "filterdefault.hpp"

namespace Flounder
{
	filterdefault::filterdefault() :
		IPostFilter("filterDefault", "res/shaders/filters/default.frag.spv", nullptr)
	{
	}

	filterdefault::~filterdefault()
	{
	}

	void filterdefault::StoreValues()
	{
	}
}
