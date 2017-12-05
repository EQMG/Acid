#include "FilterCombine.hpp"

namespace Flounder
{
	FilterCombine::FilterCombine(const int &subpass) :
		IPostFilter("filterCombine", "res/shaders/filters/combine.frag.spv", subpass, {}),
		m_slideSpaceValue(new Vector4(0.0f, 1.0f, 0.0f, 1.0f))
	{
	}

	FilterCombine::~FilterCombine()
	{
		delete m_slideSpaceValue;
	}

	void FilterCombine::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
#if 0
		// TODO: Load and store as seperate floats.
		m_shader->loadUniform4f("slideSpace", *m_slideSpaceValue);
#endif
	}
}
