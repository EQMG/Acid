#include "FilterEmboss.hpp"

namespace Flounder
{
	FilterEmboss::FilterEmboss(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Emboss.frag.spv", subpass, {})
	{
	}

	FilterEmboss::~FilterEmboss()
	{
	}

	void FilterEmboss::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
