#include "FilterSepia.hpp"

namespace Flounder
{
	FilterSepia::FilterSepia(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Sepia.frag.spv", subpass, {})
	{
	}

	FilterSepia::~FilterSepia()
	{
	}

	void FilterSepia::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
	}
}
