#include "FilterBloom1.hpp"

namespace Flounder
{
	FilterBloom1::FilterBloom1() :
		IPostFilter("filterBloom1", "res/shaders/filters/bloom1.frag.spv")
	{
	}

	FilterBloom1::~FilterBloom1()
	{
	}

	void FilterBloom1::StoreValues()
	{
	}
}
