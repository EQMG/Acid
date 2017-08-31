#include "filteremboss.hpp"

namespace Flounder
{
	filteremboss::filteremboss() :
		ipostfilter("filterEmboss", "res/shaders/filters/emboss.frag.spv")
	{
	}

	filteremboss::~filteremboss()
	{
	}

	void filteremboss::storeValues()
	{
	}
}
