#include "FilterWobble.hpp"

namespace Flounder
{
	FilterWobble::FilterWobble(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Wobble.frag.spv", subpass, {}),
		m_wobbleSpeed(2.0f),
		m_wobbleAmount(0.0f)
	{
	}

	FilterWobble::~FilterWobble()
	{
	}

	void FilterWobble::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
#if 0
		m_wobbleAmount += m_wobbleSpeed * Engine::Get()->getDeltaRender();
		m_shader->loadUniform1f("moveIt", m_wobbleAmount);
#endif
	}
}
