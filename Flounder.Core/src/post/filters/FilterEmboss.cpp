#include "FilterEmboss.hpp"

namespace Flounder
{
	FilterEmboss::FilterEmboss() :
		IPostFilter("filterEmboss", "res/shaders/filters/emboss.frag.spv")
	{
	}

	FilterEmboss::~FilterEmboss()
	{
	}

	void FilterEmboss::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
