#include "filtergrey.hpp"

namespace Flounder
{
	filtergrey::filtergrey() :
		ipostfilter("filterGrey", "res/shaders/filters/grey.frag.spv")
	{
	}

	filtergrey::~filtergrey()
	{
	}

	void filtergrey::storeValues()
	{
	}
}
