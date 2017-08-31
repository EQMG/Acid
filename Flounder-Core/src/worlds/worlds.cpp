#include "worlds.hpp"

namespace Flounder
{
	worlds::worlds() :
		IModule(),
		m_noise(new noisefast(420)),
		m_driverDay(new driverlinear(0.0f, 1.0f, 100.0f)),
		m_factorDay(0.0f),
		m_sunPosition(new vector3()),
		m_sunColour(new colour())
	{
		m_noise->setNoiseType(noisefast::SimplexFractal);
		m_noise->setInterp(noisefast::Quintic);
		m_noise->setFrequency(0.005f);
		m_noise->setFractalOctaves(9);
		m_noise->setFractalLacunarity(1.5f);
		m_noise->setFractalGain(0.7f);
	}

	worlds::~worlds()
	{
		delete m_noise;

		delete m_driverDay;

		delete m_sunPosition;
		delete m_sunColour;
	}

	void worlds::Update()
	{
		float delta = Engine::Get()->GetDelta();
		m_factorDay = m_driverDay->update(delta);

		vector3 skyboxRotation = vector3(360.0f * m_factorDay, 0.0f, 0.0f);
		vector3 lightDirection = vector3();
		colour fogColour = colour();

		matrix4x4::rotate(vector3(0.2f, 0.0f, 0.5f), skyboxRotation, &lightDirection);
		lightDirection.normalize();

		colour::interpolate(colour(0.9f, 0.3f, 0.3f, 1.0f), colour(0.05f, 0.05f, 0.1f, 1.0f), getSunriseFactor(), &fogColour);
		colour::interpolate(fogColour, colour(0.0f, 0.3f, 0.7f, 1.0f), getShadowFactor(), &fogColour);

		vector3::multiply(lightDirection, vector3(-250.0f, -250.0f, -250.0f), m_sunPosition);

		if (Camera::Get() != nullptr && Camera::Get()->GetCamera() != nullptr)
		{
			vector3::add(*m_sunPosition, *Camera::Get()->GetCamera()->GetPosition(), m_sunPosition);
		}

		colour::interpolate(colour(0.9f, 0.3f, 0.3f, 1.0f), colour(0.0f, 0.0f, 0.0f, 1.0f), getSunriseFactor(), m_sunColour);
		colour::interpolate(*m_sunColour, colour(1.0f, 1.0f, 1.0f, 1.0f), getShadowFactor(), m_sunColour);

		if (skyboxes::get() != nullptr && skyboxes::get()->getSkybox() != nullptr)
		{
			skyboxes::get()->getSkybox()->getRotation()->set(skyboxRotation);
			skyboxes::get()->getSkybox()->setBlend(starIntensity());
		}

		if (skyboxes::get() != nullptr && skyboxes::get()->getFog() != nullptr)
		{
			skyboxes::get()->getFog()->m_density = 0.006f + ((1.0f - getShadowFactor()) * 0.006f);
			skyboxes::get()->getFog()->m_gradient = 2.80f - ((1.0f - getShadowFactor()) * 0.4f);
			skyboxes::get()->getFog()->m_colour->set(fogColour);
		}

		if (shadows::get() != nullptr)
		{
			shadows::get()->getLightDirection()->set(lightDirection);
			shadows::get()->setShadowBoxOffset((20.0f * (1.0f - getShadowFactor())) + 10.0f);
			shadows::get()->setShadowBoxDistance(35.0f);
			shadows::get()->setShadowTransition(0.0f);
			shadows::get()->setShadowFactor(getShadowFactor());
		}
	}

	float worlds::getDayFactor()
	{
		return m_factorDay;
	}

	float worlds::getSunriseFactor()
	{
		return maths::clamp(-(sin(2.0f * PI * getDayFactor()) - 1.0f) / 2.0f, 0.0f, 1.0f);
	}

	float worlds::getShadowFactor()
	{
		return maths::clamp(1.7f * sin(2.0f * PI * getDayFactor()), 0.0f, 1.0f);
	}

	float worlds::getSunHeight()
	{
		return 0.0f; // TODO
	}

	float worlds::starIntensity()
	{
		return maths::clamp(1.0f - getShadowFactor(), 0.0f, 1.0f);
	}
}
