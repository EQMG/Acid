#include "filterbloom1.hpp"

namespace Flounder
{
	filterbloom1::filterbloom1() :
		IPostFilter("filterBloom1", "res/shaders/filters/bloom1.frag.spv")
	{
	}

	filterbloom1::~filterbloom1()
	{
	}

	void filterbloom1::StoreValues()
	{
	}
}
