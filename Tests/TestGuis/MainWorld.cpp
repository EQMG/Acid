#include "MainWorld.hpp"

#include <Scenes/Scenes.hpp>
#include <Shadows/Shadows.hpp>

namespace test
{
	MainWorld::MainWorld() :
		m_noiseTerrain(Noise(69124)),
		m_fog(Fog(Colour::WHITE, 0.001f, 2.0f, -0.1f, 0.3f)),
		m_skyColour(Colour("#3399ff"))
	{
	}

	MainWorld::~MainWorld()
	{
	}

	void MainWorld::Update()
	{
	}

	float MainWorld::GetDayFactor() const
	{
		return 0.0f;
	}

	float MainWorld::GetSunriseFactor() const
	{
		return 0.0f;
	}

	float MainWorld::GetShadowFactor() const
	{
		return 0.0f;
	}

	float MainWorld::GetStarIntensity() const
	{
		return 0.0f;
	}

	float MainWorld::GetTerrainRadius(const float &radius, const float &theta, const float &phi)
	{
		return 0.0f;
	}
}
