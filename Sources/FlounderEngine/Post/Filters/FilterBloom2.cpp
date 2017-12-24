#include "FilterBloom2.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
	{
	};

	FilterBloom2::FilterBloom2(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Bloom2.frag.spv", subpass, DESCRIPTORS)
	{
	}

	FilterBloom2::~FilterBloom2()
	{
	}

	void FilterBloom2::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
	}
}
