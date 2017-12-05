#include "FilterNegative.hpp"

namespace Flounder
{
	FilterNegative::FilterNegative(const int &subpass) :
		IPostFilter("filterNegative", "res/shaders/filters/negative.frag.spv", subpass, {})
	{
	}

	FilterNegative::~FilterNegative()
	{
	}

	void FilterNegative::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
