#include "FilterBloom2.hpp"

namespace Flounder
{
	FilterBloom2::FilterBloom2(const int &subpass) :
		IPostFilter("filterBloom2", "res/shaders/filters/bloom2.frag.spv", subpass, {})
	{
	}

	FilterBloom2::~FilterBloom2()
	{
	}

	void FilterBloom2::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
