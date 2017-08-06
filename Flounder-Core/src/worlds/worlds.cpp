#include "worlds.h"

namespace flounder
{
	worlds::worlds() :
		imodule()
	{
		m_noise = new noisefast(420);
		m_noise->setNoiseType(noisefast::PerlinFractal);
		m_noise->setInterp(noisefast::Quintic);
		m_noise->setFractalOctaves(3);
		m_noise->setFractalLacunarity(1.8f);
		m_noise->setFractalGain(0.6f);

		m_driverDay = new driverlinear(0.0f, 1.0f, 100.0f);
		m_factorDay = 0.0f;

		m_sunPosition = new vector3();
		m_sunColour = new colour();
	}

	worlds::~worlds()
	{
		delete m_noise;

		delete m_driverDay;

		delete m_sunPosition;
		delete m_sunColour;
	}

	void worlds::update()
	{
		float delta = framework::get()->getDelta();
		skybox *skybox = skyboxes::get()->getSkybox();
		fog *fog = skyboxes::get()->getFog();
		vector3 *lightPosition = shadows::get()->getLightPosition();
		m_factorDay = 0.25f; //  m_driverDay->update(delta);

		skybox->getRotation()->set(360.0f * m_factorDay, 0.0f, 0.0f);
		matrix4x4::rotate(vector3(0.2f, 0.0f, 0.5f), *skybox->getRotation(), lightPosition);
		lightPosition->normalize();

		colour::interpolate(colour(0.9f, 0.3f, 0.3f), colour(0.05f, 0.05f, 0.1f), getSunriseFactor(), fog->m_colour);
		colour::interpolate(*fog->m_colour, colour(0.0f, 0.3f, 0.7f), getShadowFactor(), fog->m_colour);
		fog->m_density = 0.006f + ((1.0f - getShadowFactor()) * 0.006f);
		fog->m_gradient = 2.80f - ((1.0f - getShadowFactor()) * 0.4f);

		skybox->setBlend(starIntensity());

		shadows::get()->setShadowBoxOffset((20.0f * (1.0f - getShadowFactor())) + 10.0f);
		shadows::get()->setShadowBoxDistance(35.0f);
		shadows::get()->setShadowTransition(0.0f);
		shadows::get()->setShadowFactor(getShadowFactor());

		vector3::multiply(*lightPosition, vector3(-250.0f, -250.0f, -250.0f), m_sunPosition);
		vector3::add(*m_sunPosition, *camera::get()->getCamera()->getPosition(), m_sunPosition);
		
		colour::interpolate(colour(0.9f, 0.3f, 0.3f), colour(0.0f, 0.0f, 0.0f), getSunriseFactor(), m_sunColour);
		colour::interpolate(*m_sunColour, colour(1.0f, 1.0f, 1.0f), getShadowFactor(), m_sunColour);
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
