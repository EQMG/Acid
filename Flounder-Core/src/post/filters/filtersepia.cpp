#include "filtersepia.hpp"

namespace flounder
{
	filtersepia::filtersepia() :
		ipostfilter("filterSepia", "res/shaders/filters/sepia.frag.spv")
	{
	}

	filtersepia::~filtersepia()
	{
	}

	void filtersepia::storeValues()
	{
	}
}
