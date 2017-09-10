#include "filtersepia.hpp"

namespace Flounder
{
	filtersepia::filtersepia() :
		IPostFilter("filterSepia", "res/shaders/filters/sepia.frag.spv")
	{
	}

	filtersepia::~filtersepia()
	{
	}

	void filtersepia::StoreValues()
	{
	}
}
