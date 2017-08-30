#include "shadows.hpp"

namespace flounder
{
	shadows::shadows() :
		imodule(),
		m_lightDirection(new vector3(0.5f, 0.0f, 0.5f)),
		m_brightnessBoost(0.1f),
		m_shadowSize(8192),
		m_shadowPCF(0),
		m_shadowBias(0.001f),
		m_shadowDarkness(0.6f),
		m_shadowTransition(11.0f),
		m_shadowBoxOffset(25.0f),
		m_shadowBoxDistance(40.0f),
		m_shadowFactor(1.0f),
		m_shadowBox(new shadowbox())
	{
	}

	shadows::~shadows()
	{
		delete m_lightDirection;

		delete m_shadowBox;
	}

	void shadows::update()
	{
		if (camera::get() != nullptr && camera::get()->getCamera() != nullptr)
		{
	//		m_shadowBox->update(*camera::get()->getCamera(), *m_lightDirection, m_shadowBoxOffset, m_shadowBoxDistance);
		}
	}
}
