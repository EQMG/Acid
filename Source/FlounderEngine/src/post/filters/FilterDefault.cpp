#include "FilterDefault.hpp"

namespace Flounder
{
	FilterDefault::FilterDefault(const int &subpass) :
		IPostFilter("filterDefault", "res/shaders/filters/default.frag.spv", subpass, {})
	{
	}

	FilterDefault::~FilterDefault()
	{
	}

	void FilterDefault::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
