#include "FilterFxaa.hpp"

namespace Flounder
{
	FilterFxaa::FilterFxaa(const float &spanMax) :
		IPostFilter("filterFxaa", "res/shaders/filters/fxaa.frag.spv"),
		m_spanMax(spanMax)
	{
	}

	FilterFxaa::~FilterFxaa()
	{
	}

	void FilterFxaa::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
#if 0
		m_shader->loadUniform1f("spanMax", m_spanMax);
#endif
	}
}
