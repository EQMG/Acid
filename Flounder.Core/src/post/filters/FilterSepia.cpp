#include "FilterSepia.hpp"

namespace Flounder
{
	FilterSepia::FilterSepia(const int &subpass) :
		IPostFilter("filterSepia", "res/shaders/filters/sepia.frag.spv", subpass, {})
	{
	}

	FilterSepia::~FilterSepia()
	{
	}

	void FilterSepia::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
