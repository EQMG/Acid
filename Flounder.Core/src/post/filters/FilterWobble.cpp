#include "filterwobble.hpp"

namespace Flounder
{
	filterwobble::filterwobble(const float &wobbleSpeed) :
		IPostFilter("filterWobble", "res/shaders/filters/wobble.frag.spv"),
		m_wobbleSpeed(wobbleSpeed)
	{
	}

	filterwobble::~filterwobble()
	{
	}

	void filterwobble::StoreValues()
	{
#if 0
		m_wobbleAmount += m_wobbleSpeed * Engine::Get()->getDeltaRender();
		m_shader->loadUniform1f("moveIt", m_wobbleAmount);
#endif
	}
}
