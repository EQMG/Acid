#include "filtertone.hpp"

namespace Flounder
{
	filtertone::filtertone() :
		IPostFilter("filterTone", "res/shaders/filters/tone.frag.spv")
	{
	}

	filtertone::~filtertone()
	{
	}

	void filtertone::StoreValues()
	{
	}
}
