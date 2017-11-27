#include "Shadows.hpp"

#include "../camera/Camera.hpp"

namespace Flounder
{
	Shadows::Shadows() :
		IModule(),
		m_lightDirection(new Vector3(0.5f, 0.0f, 0.5f)),
		m_brightnessBoost(0.1f),
		m_shadowSize(8192),
		m_shadowPcf(0),
		m_shadowBias(0.001f),
		m_shadowDarkness(0.6f),
		m_shadowTransition(11.0f),
		m_shadowBoxOffset(25.0f),
		m_shadowBoxDistance(40.0f),
		m_shadowFactor(1.0f),
		m_shadowBox(new ShadowBox())
	{
	}

	Shadows::~Shadows()
	{
		delete m_lightDirection;

		delete m_shadowBox;
	}

	void Shadows::Update()
	{
		//	if (Camera::Get() != nullptr && Camera::Get()->GetCamera() != nullptr)
		//	{
		//		m_shadowBox->Update(*Camera::Get()->GetCamera(), *m_lightDirection, m_shadowBoxOffset, m_shadowBoxDistance);
		//	}
	}
}
