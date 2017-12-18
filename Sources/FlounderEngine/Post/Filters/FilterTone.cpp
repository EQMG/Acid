#include "FilterTone.hpp"

namespace Flounder
{
	FilterTone::FilterTone(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Tone.frag.spv", subpass, {})
	{
	}

	FilterTone::~FilterTone()
	{
	}

	void FilterTone::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
	}
}
