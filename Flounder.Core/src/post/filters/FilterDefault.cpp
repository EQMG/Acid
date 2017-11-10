#include "FilterDefault.hpp"

namespace Flounder
{
	FilterDefault::FilterDefault() :
		IPostFilter("filterDefault", "res/shaders/filters/default.frag.spv")
	{
	}

	FilterDefault::~FilterDefault()
	{
	}

	void FilterDefault::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
