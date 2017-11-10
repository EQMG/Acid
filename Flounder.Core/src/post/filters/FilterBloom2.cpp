#include "FilterBloom2.hpp"

namespace Flounder
{
	FilterBloom2::FilterBloom2() :
		IPostFilter("filterBloom2", "res/shaders/filters/bloom2.frag.spv")
	{
	}

	FilterBloom2::~FilterBloom2()
	{
	}

	void FilterBloom2::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
