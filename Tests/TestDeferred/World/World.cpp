#include "World.hpp"

#include <Post/Deferred/RendererDeferred.hpp>
#include <Shadows/Shadows.hpp>

namespace test
{
	static const Colour FOG_COLOUR_SUNRISE = Colour("#ee9a90");
	static const Colour FOG_COLOUR_NIGHT = Colour("#0D0D1A");
	static const Colour FOG_COLOUR_DAY = Colour("#e6e6e6");

	World::World() :
		IModule(),
		m_driverDay(DriverLinear(0.0f, 1.0f, 300.0f)),
		m_factorDay(0.0f),
		m_fog(Fog(Colour::WHITE, 0.001f, 2.0f, -0.1f, 0.3f)),
		m_skyboxRotation(Vector3()),
		m_lightDirection(Vector3())
	{
		m_driverDay.Update(50.0f); // Starts during daytime.
	}

	World::~World()
	{
	}

	void World::Update()
	{
		float delta = Engine::Get()->GetDelta();
		m_factorDay = m_driverDay.Update(delta);

		m_skyboxRotation = Vector3(360.0f * m_factorDay, 0.0f, 0.0f);

		m_lightDirection = m_skyboxRotation.Rotate(Vector3(0.2f, 0.0f, 0.5f));
		m_lightDirection.Normalize();

		Colour fogColour = FOG_COLOUR_SUNRISE.Interpolate(FOG_COLOUR_NIGHT, GetSunriseFactor());
		fogColour = fogColour.Interpolate(FOG_COLOUR_DAY, GetShadowFactor());
		m_fog.SetColour(fogColour);
		m_fog.SetDensity(0.002f + ((1.0f - GetShadowFactor()) * 0.002f));
		m_fog.SetGradient(2.0f - ((1.0f - GetShadowFactor()) * 0.380f));
		m_fog.SetLowerLimit(0.0f);
		m_fog.SetUpperLimit(0.15f - ((1.0f - GetShadowFactor()) * 0.03f));

		auto deferred = Renderer::Get()->GetManager()->GetRenderer<RendererDeferred>();

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
		return Maths::Clamp(-(std::sin(2.0f * PI * GetDayFactor()) - 1.0f) / 2.0f, 0.0f, 1.0f);
	}

	float World::GetShadowFactor() const
	{
		return Maths::Clamp(1.7f * std::sin(2.0f * PI * GetDayFactor()), 0.0f, 1.0f);
	}

	float World::GetStarIntensity() const
	{
		return Maths::Clamp(1.0f - GetShadowFactor(), 0.0f, 1.0f);
	}
}