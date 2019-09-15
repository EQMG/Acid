#pragma once

#include <Engine/Engine.hpp>
#include <Maths/Visual/DriverLinear.hpp>
#include <Maths/Vector3.hpp>
#include <Lights/Fog.hpp>

using namespace acid;

namespace test {
/**
 * Module used for managing the world.
 */
class World :
	public Module {
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static World *Get() { return Engine::Get()->GetModule<World>(); }

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
	DriverLinear<float> m_driverDay;
	float m_factorDay;

	Fog m_fog;

	Vector3f m_skyboxRotation;
	Vector3f m_lightDirection;
};
}
