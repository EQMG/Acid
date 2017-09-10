#include "filternegative.hpp"

namespace Flounder
{
	filternegative::filternegative() :
		IPostFilter("filterNegative", "res/shaders/filters/negative.frag.spv")
	{
	}

	filternegative::~filternegative()
	{
	}

	void filternegative::StoreValues()
	{
	}
}
