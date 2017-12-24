#include "FilterDefault.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
	{
	};

	FilterDefault::FilterDefault(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Default.frag.spv", subpass, DESCRIPTORS)
	{
	}

	FilterDefault::~FilterDefault()
	{
	}

	void FilterDefault::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
	}
}
