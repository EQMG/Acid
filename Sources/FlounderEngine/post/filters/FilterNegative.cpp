#include "FilterNegative.hpp"

namespace Flounder
{
	FilterNegative::FilterNegative(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Negative.frag.spv", subpass, {})
	{
	}

	FilterNegative::~FilterNegative()
	{
	}

	void FilterNegative::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
