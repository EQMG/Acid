#include "FilterEmboss.hpp"

namespace Flounder
{
	FilterEmboss::FilterEmboss(const int &subpass) :
		IPostFilter("filterEmboss", "res/shaders/filters/emboss.frag.spv", subpass, {})
	{
	}

	FilterEmboss::~FilterEmboss()
	{
	}

	void FilterEmboss::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
