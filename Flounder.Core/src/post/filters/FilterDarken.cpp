#include "FilterDarken.hpp"

namespace Flounder
{
	FilterDarken::FilterDarken(const float &factorValue) :
		IPostFilter("filterDarken", "res/shaders/filters/darken.frag.spv"),
		m_factorValue(factorValue)
	{
	}

	FilterDarken::FilterDarken() :
		FilterDarken(0.45f)
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
