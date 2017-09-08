#include "Worlds.hpp"

namespace Flounder
{
	Worlds::Worlds() :
		IModule(),
		m_noise(new NoiseFast(420)),
		m_driverDay(new driverlinear(0.0f, 1.0f, 100.0f)),
		m_factorDay(0.0f),
		m_sunPosition(new Vector3()),
		m_sunColour(new Colour())
	{
		m_noise->SetNoiseType(NoiseFast::SimplexFractal);
		m_noise->SetInterp(NoiseFast::Quintic);
		m_noise->SetFrequency(0.005f);
		m_noise->SetFractalOctaves(9);
		m_noise->SetFractalLacunarity(1.5f);
		m_noise->SetFractalGain(0.7f);
	}

	Worlds::~Worlds()
	{
		delete m_noise;

		delete m_driverDay;

		delete m_sunPosition;
		delete m_sunColour;
	}

	void Worlds::Update()
	{
		float delta = Engine::Get()->GetDelta();
		m_factorDay = m_driverDay->update(delta);

		Vector3 skyboxRotation = Vector3(360.0f * m_factorDay, 0.0f, 0.0f);
		Vector3 lightDirection = Vector3();
		Colour fogColour = Colour();

		Matrix4::Rotate(Vector3(0.2f, 0.0f, 0.5f), skyboxRotation, &lightDirection);
		lightDirection.Normalize();

		Colour::Interpolate(Colour(0.9f, 0.3f, 0.3f, 1.0f), Colour(0.05f, 0.05f, 0.1f, 1.0f), GetSunriseFactor(), &fogColour);
		Colour::Interpolate(fogColour, Colour(0.0f, 0.3f, 0.7f, 1.0f), GetShadowFactor(), &fogColour);

		Vector3::Multiply(lightDirection, Vector3(-250.0f, -250.0f, -250.0f), m_sunPosition);

		if (Camera::Get() != nullptr && Camera::Get()->GetCamera() != nullptr)
		{
			Vector3::Add(*m_sunPosition, *Camera::Get()->GetCamera()->GetPosition(), m_sunPosition);
		}

		Colour::Interpolate(Colour(0.9f, 0.3f, 0.3f, 1.0f), Colour(0.0f, 0.0f, 0.0f, 1.0f), GetSunriseFactor(), m_sunColour);
		Colour::Interpolate(*m_sunColour, Colour(1.0f, 1.0f, 1.0f, 1.0f), GetShadowFactor(), m_sunColour);

		if (skyboxes::get() != nullptr && skyboxes::get()->getSkybox() != nullptr)
		{
			skyboxes::get()->getSkybox()->getRotation()->Set(skyboxRotation);
			skyboxes::get()->getSkybox()->setBlend(GetStarIntensity());
		}

		if (skyboxes::get() != nullptr && skyboxes::get()->getFog() != nullptr)
		{
			skyboxes::get()->getFog()->m_density = 0.006f + ((1.0f - GetShadowFactor()) * 0.006f);
			skyboxes::get()->getFog()->m_gradient = 2.80f - ((1.0f - GetShadowFactor()) * 0.4f);
			skyboxes::get()->getFog()->m_colour->Set(fogColour);
		}

		if (shadows::get() != nullptr)
		{
			shadows::get()->getLightDirection()->Set(lightDirection);
			shadows::get()->setShadowBoxOffset((20.0f * (1.0f - GetShadowFactor())) + 10.0f);
			shadows::get()->setShadowBoxDistance(35.0f);
			shadows::get()->setShadowTransition(0.0f);
			shadows::get()->setShadowFactor(GetShadowFactor());
		}
	}

	float Worlds::GetDayFactor()
	{
		return m_factorDay;
	}

	float Worlds::GetSunriseFactor()
	{
		return Maths::Clamp(-(sin(2.0f * PI * GetDayFactor()) - 1.0f) / 2.0f, 0.0f, 1.0f);
	}

	float Worlds::GetShadowFactor()
	{
		return Maths::Clamp(1.7f * sin(2.0f * PI * GetDayFactor()), 0.0f, 1.0f);
	}

	float Worlds::GetSunHeight()
	{
		return 0.0f; // TODO
	}

	float Worlds::GetStarIntensity()
	{
		return Maths::Clamp(1.0f - GetShadowFactor(), 0.0f, 1.0f);
	}
}
