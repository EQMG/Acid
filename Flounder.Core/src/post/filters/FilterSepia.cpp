#include "filtersepia.hpp"

namespace Flounder
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
