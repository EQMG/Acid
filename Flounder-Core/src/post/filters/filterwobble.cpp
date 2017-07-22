#include "filterwobble.h"

namespace flounder
{
	filterwobble::filterwobble(const float &wobbleSpeed) :
		ipostfilter("filterWobble", "res/shaders/filters/wobbleFragment.glsl")
	{
		m_wobbleSpeed = wobbleSpeed;
	}

	filterwobble::filterwobble() :
		filterwobble(2.0f)
	{
	}

	filterwobble::~filterwobble()
	{
	}

	void filterwobble::storeValues()
	{
		m_wobbleAmount += m_wobbleSpeed * framework::get()->getDeltaRender();
		m_shader->loadUniform("moveIt", m_wobbleAmount);
	}
}
