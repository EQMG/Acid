#include "FilterDarken.hpp"

namespace Flounder
{
	FilterDarken::FilterDarken(const int &subpass) :
		IPostFilter("filterDarken", "res/shaders/filters/darken.frag.spv", subpass, {}),
		m_factorValue(0.45f)
	{
	}

	FilterDarken::~FilterDarken()
	{
	}

	void FilterDarken::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
#if 0
		m_shader->loadUniform1f("factor", m_factorValue);
#endif
	}
}
