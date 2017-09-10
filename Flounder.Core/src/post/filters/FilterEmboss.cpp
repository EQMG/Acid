#include "filteremboss.hpp"

namespace Flounder
{
	filteremboss::filteremboss() :
		IPostFilter("filterEmboss", "res/shaders/filters/emboss.frag.spv")
	{
	}

	filteremboss::~filteremboss()
	{
	}

	void filteremboss::StoreValues()
	{
	}
}
