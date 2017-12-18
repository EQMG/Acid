#include "FilterBloom2.hpp"

namespace Flounder
{
	FilterBloom2::FilterBloom2(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Bloom2.frag.spv", subpass, {})
	{
	}

	FilterBloom2::~FilterBloom2()
	{
	}

	void FilterBloom2::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
	}
}
