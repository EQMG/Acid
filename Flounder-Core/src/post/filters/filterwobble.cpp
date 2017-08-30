#include "filterwobble.hpp"

namespace flounder
{
	filterwobble::filterwobble(const float &wobbleSpeed) :
		ipostfilter("filterWobble", "res/shaders/filters/wobble.frag.spv"),
		m_wobbleSpeed(wobbleSpeed)
	{
	}

	filterwobble::~filterwobble()
	{
	}

	void filterwobble::storeValues()
	{
#if 0
		m_wobbleAmount += m_wobbleSpeed * framework::get()->getDeltaRender();
		m_shader->loadUniform1f("moveIt", m_wobbleAmount);
#endif
	}
}
