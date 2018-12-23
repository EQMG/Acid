#include "World.hpp"

#include <Post/Deferred/RendererDeferred.hpp>
#include <Shadows/Shadows.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>

namespace test
{
	static const Colour FOG_COLOUR_SUNRISE = Colour("#ee9a90");
	static const Colour FOG_COLOUR_NIGHT = Colour("#0D0D1A");
	static const Colour FOG_COLOUR_DAY = Colour("#e6e6e6");

	World::World() :
		m_driverDay(DriverLinear(0.0f, 1.0f, Time::Seconds(300.0f))),
		m_factorDay(0.0f),
		m_fog(Fog(Colour::WHITE, 0.001f, 2.0f, -0.1f, 0.3f)),
		m_skyboxRotation(Vector3()),
		m_lightDirection(Vector3())
	{
		m_driverDay.Update(Time::Seconds(50.0f)); // Starts during daytime.
	}

	void World::Update()
	{
		m_factorDay = m_driverDay.Update(Engine::Get()->GetDelta());

		m_skyboxRotation = Vector3(360.0f * m_factorDay, 0.0f, 0.0f);

		m_lightDirection = m_skyboxRotation.Rotate(Vector3(0.154303f, 0.771517f, -0.617213f));
		m_lightDirection.Normalize();

		Colour fogColour = FOG_COLOUR_SUNRISE.Lerp(FOG_COLOUR_NIGHT, GetSunriseFactor());
		fogColour = fogColour.Lerp(FOG_COLOUR_DAY, GetShadowFactor());
		m_fog.SetColour(fogColour);
		m_fog.SetDensity(0.002f + ((1.0f - GetShadowFactor()) * 0.002f));
		m_fog.SetGradient(2.0f - ((1.0f - GetShadowFactor()) * 0.380f));
		m_fog.SetLowerLimit(0.0f);
		m_fog.SetUpperLimit(0.15f - ((1.0f - GetShadowFactor()) * 0.03f));

		auto deferred = Renderer::Get()->GetRendererRegister().Get<RendererDeferred>();

		if (deferred != nullptr)
		{
			deferred->SetFog(m_fog);
		}

		if (Shadows::Get() != nullptr)
		{
			Shadows::Get()->SetLightDirection(m_lightDirection);
			Shadows::Get()->SetShadowBoxOffset((4.0f * (1.0f - GetShadowFactor())) + 10.0f);
			Shadows::Get()->SetShadowBoxDistance(40.0f);
			Shadows::Get()->SetShadowTransition(5.0f);
			Shadows::Get()->SetShadowDarkness(0.6f * GetShadowFactor());
		}
	}

	float World::GetDayFactor() const
	{
		return m_factorDay;
	}

	float World::GetSunriseFactor() const
	{
		return std::clamp(-(std::sin(2.0f * PI * GetDayFactor()) - 1.0f) / 2.0f, 0.0f, 1.0f);
	}

	float World::GetShadowFactor() const
	{
		return std::clamp(1.7f * std::sin(2.0f * PI * GetDayFactor()), 0.0f, 1.0f);
	}

	float World::GetStarIntensity() const
	{
		return std::clamp(1.0f - GetShadowFactor(), 0.0f, 1.0f);
	}
}