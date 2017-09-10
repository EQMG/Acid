#include "FilterDefault.hpp"

namespace Flounder
{
	FilterDefault::FilterDefault() :
		IPostFilter("filterDefault", "res/shaders/filters/default.frag.spv", nullptr)
	{
	}

	FilterDefault::~FilterDefault()
	{
	}

	void FilterDefault::StoreValues()
	{
	}
}
