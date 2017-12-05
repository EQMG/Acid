#include "FilterTone.hpp"

namespace Flounder
{
	FilterTone::FilterTone(const int &subpass) :
		IPostFilter("filterTone", "res/shaders/filters/tone.frag.spv", subpass, {})
	{
	}

	FilterTone::~FilterTone()
	{
	}

	void FilterTone::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
