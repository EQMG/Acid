#include "filtergrey.hpp"

namespace Flounder
{
	filtergrey::filtergrey() :
		IPostFilter("filterGrey", "res/shaders/filters/grey.frag.spv")
	{
	}

	filtergrey::~filtergrey()
	{
	}

	void filtergrey::StoreValues()
	{
	}
}
