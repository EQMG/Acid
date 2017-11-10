#include "FilterNegative.hpp"

namespace Flounder
{
	FilterNegative::FilterNegative() :
		IPostFilter("filterNegative", "res/shaders/filters/negative.frag.spv")
	{
	}

	FilterNegative::~FilterNegative()
	{
	}

	void FilterNegative::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
