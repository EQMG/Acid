#include "FilterSepia.hpp"

namespace Flounder
{
	FilterSepia::FilterSepia() :
		IPostFilter("filterSepia", "res/shaders/filters/sepia.frag.spv")
	{
	}

	FilterSepia::~FilterSepia()
	{
	}

	void FilterSepia::StoreValues()
	{
	}
}
