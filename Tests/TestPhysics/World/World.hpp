#pragma once

#include <Engine/Engine.hpp>
#include <Uis/Drivers/LinearDriver.hpp>
#include <Maths/Vector3.hpp>
#include <Lights/Fog.hpp>

using namespace acid;

namespace test {
class World : public Module::Registrar<World, Module::Stage::Always> {
public:
	World();

	void Update() override;

	float GetDayFactor() const;
	float GetSunriseFactor() const;
	float GetShadowFactor() const;
	float GetStarIntensity() const;

	Fog GetFog() const { return m_fog; }
	Vector3f GetSkyboxRotation() const { return m_skyboxRotation; }
	Vector3f GetLightDirection() const { return m_lightDirection; }

private:
	LinearDriver<float> m_driverDay;

	Fog m_fog;

	Vector3f m_skyboxRotation;
	Vector3f m_lightDirection;
};
}
