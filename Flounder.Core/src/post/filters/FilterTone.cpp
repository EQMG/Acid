#include "FilterTone.hpp"

namespace Flounder
{
	FilterTone::FilterTone() :
		IPostFilter("filterTone", "res/shaders/filters/tone.frag.spv")
	{
	}

	FilterTone::~FilterTone()
	{
	}

	void FilterTone::StoreValues()
	{
	}
}
