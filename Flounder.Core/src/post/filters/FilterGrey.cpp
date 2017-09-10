#include "FilterGrey.hpp"

namespace Flounder
{
	FilterGrey::FilterGrey() :
		IPostFilter("filterGrey", "res/shaders/filters/grey.frag.spv")
	{
	}

	FilterGrey::~FilterGrey()
	{
	}

	void FilterGrey::StoreValues()
	{
	}
}
