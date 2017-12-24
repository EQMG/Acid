#include "FilterBloom1.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
	{
	};

	FilterBloom1::FilterBloom1(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Bloom1.frag.spv", subpass, DESCRIPTORS)
	{
	}

	FilterBloom1::~FilterBloom1()
	{
	}

	void FilterBloom1::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
	}
}
