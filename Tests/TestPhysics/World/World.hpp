#pragma once

#include <Engine/Engine.hpp>
#include <Uis/Drivers/LinearDriver.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Maths/Vector3.hpp>
#include <Lights/Fog.hpp>

using namespace acid;

namespace test {
class World : public Module::Registrar<World> {
	inline static const bool Registered = Register(Stage::Always, Requires<Graphics, Scenes>());
public:
	World();

	void Update() override;

	float GetDayFactor() const;
	float GetSunriseFactor() const;
	float GetShadowFactor() const;
	float GetStarIntensity() const;

	const Fog &GetFog() const { return fog; }
	const Vector3f &GetSkyboxRotation() const { return skyboxRotation; }
	const Vector3f &GetLightDirection() const { return lightDirection; }

private:
	LinearDriver<float> driverDay;

	Fog fog;

	Vector3f skyboxRotation;
	Vector3f lightDirection;
};
}
