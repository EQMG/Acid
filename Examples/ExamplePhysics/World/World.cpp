#include "World.hpp"

#include <Post/Deferred/DeferredSubrender.hpp>
#include <Post/Filters/LensflareFilter.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/Shadows.hpp>

namespace test {
constexpr static Colour FOG_COLOUR_SUNRISE = 0xEE9A90;
constexpr static Colour FOG_COLOUR_NIGHT = 0x0D0D1A;
constexpr static Colour FOG_COLOUR_DAY = 0xE6E6E6;

World::World() :
	driverDay(LinearDriver<float>(0.0f, 1.0f, 300s)),
	fog(Colour::White, 0.001f, 2.0f, -0.1f, 0.3f) {
	driverDay.Update(50s); // Starts during daytime.
}

void World::Update() {
	driverDay.Update(Engine::Get()->GetDelta());

	skyboxRotation = {360.0f * driverDay.Get(), 0.0f, 0.0f};
	lightDirection = {0.154303f, 0.771517f, -0.617213f};

	auto fogColour = FOG_COLOUR_SUNRISE.Lerp(FOG_COLOUR_NIGHT, GetSunriseFactor());
	fogColour = fogColour.Lerp(FOG_COLOUR_DAY, GetShadowFactor());
	fog.SetColour(fogColour);
	fog.SetDensity(0.002f + ((1.0f - GetShadowFactor()) * 0.002f));
	fog.SetGradient(2.0f - ((1.0f - GetShadowFactor()) * 0.380f));
	fog.SetLowerLimit(0.0f);
	fog.SetUpperLimit(0.15f - ((1.0f - GetShadowFactor()) * 0.03f));

	/*if (auto lensflare = Graphics::Get()->GetSubrender<LensflareFilter>()) {
		lensflare->SetSunPosition(Vector3f(1000.0f, 5000.0f, -4000.0f));
		lensflare->SetSunHeight(1000.0f);
	}*/

	if (auto deferred = Graphics::Get()->GetRenderer()->GetSubrender<DeferredSubrender>()) {
		deferred->SetFog(fog);
	}

	if (auto shadows = Scenes::Get()->GetScene()->GetSystem<Shadows>()) {
		shadows->SetLightDirection(-lightDirection);
		//shadows->SetShadowBoxOffset((4.0f * (1.0f - GetShadowFactor())) + 10.0f);
		//shadows->SetShadowBoxDistance(40.0f);
		//shadows->SetShadowTransition(5.0f);
		//shadows->SetShadowDarkness(0.6f * GetShadowFactor());
	}
}

float World::GetDayFactor() const {
	return driverDay.Get();
}

float World::GetSunriseFactor() const {
	return std::clamp(-(std::sin(2.0f * Maths::PI<float> * GetDayFactor()) - 1.0f) / 2.0f, 0.0f, 1.0f);
}

float World::GetShadowFactor() const {
	return std::clamp(1.7f * std::sin(2.0f * Maths::PI<float> * GetDayFactor()), 0.0f, 1.0f);
}

float World::GetStarIntensity() const {
	return std::clamp(1.0f - GetShadowFactor(), 0.0f, 1.0f);
}
}
