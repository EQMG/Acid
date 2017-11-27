#include "FilterBloom1.hpp"

#include "../../devices/Display.hpp"

namespace Flounder
{
	FilterBloom1::FilterBloom1(const int &subpass) :
		IPostFilter("filterBloom1", "res/shaders/filters/bloom1.frag.spv", subpass, {})
	{
	}

	FilterBloom1::~FilterBloom1()
	{
	}

	void FilterBloom1::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
