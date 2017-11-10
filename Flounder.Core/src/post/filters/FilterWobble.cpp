#include "FilterWobble.hpp"

#include "../../devices/Display.hpp"
#include "../../engine/Engine.hpp"

namespace Flounder
{
	FilterWobble::FilterWobble(const float &wobbleSpeed) :
		IPostFilter("filterWobble", "res/shaders/filters/wobble.frag.spv"),
		m_wobbleSpeed(wobbleSpeed),
		m_wobbleAmount(0.0f)
	{
	}

	FilterWobble::~FilterWobble()
	{
	}

	void FilterWobble::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
#if 0
		m_wobbleAmount += m_wobbleSpeed * Engine::Get()->getDeltaRender();
		m_shader->loadUniform1f("moveIt", m_wobbleAmount);
#endif
	}
}
