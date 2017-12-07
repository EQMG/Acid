#include "FilterDefault.hpp"

namespace Flounder
{
	FilterDefault::FilterDefault(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Default.frag.spv", subpass, {})
	{
	}

	FilterDefault::~FilterDefault()
	{
	}

	void FilterDefault::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
