#include "FilterCombine.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
	{
	};

	FilterCombine::FilterCombine(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Combine.frag.spv", subpass, DESCRIPTORS),
		m_slideSpaceValue(new Vector4(0.0f, 1.0f, 0.0f, 1.0f))
	{
	}

	FilterCombine::~FilterCombine()
	{
		delete m_slideSpaceValue;
	}

	void FilterCombine::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
#if 0
		m_shader->loadUniform4f("slideSpace", *m_slideSpaceValue);
#endif
	}
}
