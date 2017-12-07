#include "Worlds.hpp"

#include "../Camera/Camera.hpp"
#include "../Maths/Matrix4.hpp"
#include "../Shadows/Shadows.hpp"

namespace Flounder
{
	Worlds::Worlds() :
		IModule(),
		m_driverDay(new DriverLinear(0.0f, 1.0f, 60.0f)),
		m_factorDay(0.0f),
		m_sunPosition(new Vector3()),
		m_sunColour(new Colour())
	{
	}

	Worlds::~Worlds()
	{
		delete m_driverDay;

		delete m_sunPosition;
		delete m_sunColour;
	}

	void Worlds::Update()
	{
		const float delta = Engine::Get()->GetDelta();
		m_factorDay = m_driverDay->Update(delta);

		Vector3 skyboxRotation = Vector3(360.0f * m_factorDay, 0.0f, 0.0f);
		Vector3 lightDirection = Vector3();
		Colour fogColour = Colour();

		Matrix4::Rotate(Vector3(0.2f, 0.0f, 0.5f), skyboxRotation, &lightDirection);
		lightDirection.Normalize();

		Colour::Interpolate(Colour("#E14938"), Colour("#0D0D1A"), GetSunriseFactor(), &fogColour);
		Colour::Interpolate(fogColour, Colour("#2C82C9"), GetShadowFactor(), &fogColour);

		Vector3::Multiply(lightDirection, Vector3(-500.0f, -500.0f, -500.0f), m_sunPosition);

		if (Camera::Get() != nullptr && Camera::Get()->GetCamera() != nullptr)
		{
			Vector3::Add(*m_sunPosition, *Camera::Get()->GetCamera()->GetPosition(), m_sunPosition);
		}

		Colour::Interpolate(Colour("#E14938"), Colour("#0D0D1A"), GetSunriseFactor(), m_sunColour);
		Colour::Interpolate(*m_sunColour, Colour("#ffffff"), GetShadowFactor(), m_sunColour);

		if (Skyboxes::Get() != nullptr && Skyboxes::Get()->GetSkybox() != nullptr)
		{
			Skyboxes::Get()->GetSkybox()->GetRotation()->Set(skyboxRotation);
			Skyboxes::Get()->GetSkybox()->SetBlend(GetStarIntensity());
		}

		if (Skyboxes::Get() != nullptr && Skyboxes::Get()->GetFog() != nullptr)
		{
			Skyboxes::Get()->GetFog()->m_density = 0.002f + ((1.0f - GetShadowFactor()) * 0.002f);
			Skyboxes::Get()->GetFog()->m_gradient = 2.20f - ((1.0f - GetShadowFactor()) * 0.380f);
			Skyboxes::Get()->GetFog()->m_colour->Set(fogColour);
		}

		if (Shadows::Get() != nullptr)
		{
			Shadows::Get()->GetLightDirection()->Set(lightDirection);
			Shadows::Get()->SetShadowBoxOffset((20.0f * (1.0f - GetShadowFactor())) + 10.0f);
			Shadows::Get()->SetShadowBoxDistance(35.0f);
			Shadows::Get()->SetShadowTransition(0.0f);
			Shadows::Get()->SetShadowFactor(GetShadowFactor());
		}
	}

	float Worlds::GetDayFactor() const
	{
		return m_factorDay;
	}

	float Worlds::GetSunriseFactor() const
	{
		return Maths::Clamp(static_cast<float>(-(sin(2.0f * PI * GetDayFactor()) - 1.0f)) / 2.0f, 0.0f, 1.0f);
	}

	float Worlds::GetShadowFactor() const
	{
		return Maths::Clamp(static_cast<float>(1.7f * sin(2.0f * PI * GetDayFactor())), 0.0f, 1.0f);
	}

	float Worlds::GetSunHeight() const
	{
		return 0.0f; // TODO
	}

	float Worlds::GetStarIntensity() const
	{
		return Maths::Clamp(1.0f - GetShadowFactor(), 0.0f, 1.0f);
	}
}
