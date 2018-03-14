#include "Worlds.hpp"

#include "../Scenes/Scenes.hpp"
#include "../Shadows/Shadows.hpp"

namespace Flounder
{
	Worlds *Worlds::S_INSTANCE = nullptr;

	float Worlds::WORLD_CURVATURE = 300.0f;

	static const Colour FOG_COLOUR_SUNRISE = Colour("#ee9a90");
	static const Colour FOG_COLOUR_NIGHT = Colour("#0D0D1A");
	static const Colour FOG_COLOUR_DAY = Colour("#e6e6e6");

	static const Colour SUN_COLOUR_SUNRISE = Colour("#ee9a90");
	static const Colour SUN_COLOUR_NIGHT = Colour("#0D0D1A");
	static const Colour SUN_COLOUR_DAY = Colour("#ffffff");

	static const Colour MOON_COLOUR_NIGHT = Colour("#666699");
	static const Colour MOON_COLOUR_DAY = Colour("#000000");

	static const Colour SKYBOX_COLOUR_DAY = Colour("#003C8A");

	Worlds::Worlds() :
		IModule(),
		m_noise(new NoiseFast(954627)),
		m_driverDay(new DriverLinear(0.0f, 1.0f, 300.0f)),
		m_factorDay(0.0f),
		m_skyboxRotation(new Vector3),
		m_sunPosition(new Vector3()),
		m_moonPosition(new Vector3()),
		m_sunColour(new Colour()),
		m_moonColour(new Colour()),
		m_fog(new Fog(new Colour(), 0.001f, 2.0f, -0.1f, 0.3f)),
		m_skyColour(new Colour("#3399ff"))
	{
		m_noise->SetNoiseType(NoiseFast::SimplexFractal);
		m_noise->SetFrequency(0.055f);
		m_noise->SetInterp(NoiseFast::Quintic);
		m_noise->SetFractalType(NoiseFast::Fbm);
		m_noise->SetFractalOctaves(3);
		m_noise->SetFractalLacunarity(2.0);
		m_noise->SetFractalGain(0.5f);

		m_driverDay->Update(50.0f); // Starts during daytime.
	}

	Worlds::~Worlds()
	{
		delete m_noise;

		delete m_driverDay;

		delete m_skyboxRotation;
		delete m_sunPosition;
		delete m_moonPosition;
		delete m_sunColour;
		delete m_moonColour;

		delete m_fog;
		delete m_skyColour;
	}

	void Worlds::Update()
	{
		const float delta = Engine::Get()->GetDelta();
		m_factorDay = m_driverDay->Update(delta);

		m_skyboxRotation->Set(360.0f * m_factorDay, 0.0f, 0.0f);
		Vector3 lightDirection = Vector3();
		Colour fogColour = Colour();

		Vector3::Rotate(Vector3(0.2f, 0.0f, 0.5f), *m_skyboxRotation, &lightDirection);
		lightDirection.Normalize();

		Colour::Interpolate(FOG_COLOUR_SUNRISE, FOG_COLOUR_NIGHT, GetSunriseFactor(), &fogColour);
		Colour::Interpolate(fogColour, FOG_COLOUR_DAY, GetShadowFactor(), &fogColour);

		Vector3::Multiply(lightDirection, Vector3(-500.0f, -500.0f, -500.0f), m_sunPosition);
		Vector3::Multiply(lightDirection, Vector3(500.0f, 500.0f, 500.0f), m_moonPosition);

		if (Scenes::Get()->GetCamera() != nullptr)
		{
			Vector3::Add(*m_sunPosition, *Scenes::Get()->GetCamera()->GetPosition(), m_sunPosition);
			Vector3::Add(*m_moonPosition, *Scenes::Get()->GetCamera()->GetPosition(), m_moonPosition);
		}

		Colour::Interpolate(SUN_COLOUR_SUNRISE, SUN_COLOUR_NIGHT, GetSunriseFactor(), m_sunColour);
		Colour::Interpolate(*m_sunColour, SUN_COLOUR_DAY, GetShadowFactor(), m_sunColour);

		Colour::Interpolate(MOON_COLOUR_NIGHT, MOON_COLOUR_DAY, GetShadowFactor(), m_moonColour);

		*m_fog->m_colour = fogColour;
		m_fog->m_density = 0.002f + ((1.0f - GetShadowFactor()) * 0.002f);
		m_fog->m_gradient = 2.0f - ((1.0f - GetShadowFactor()) * 0.380f);
		m_fog->m_lowerLimit = 0.0f;
		m_fog->m_upperLimit = 0.15f - ((1.0f - GetShadowFactor()) * 0.03f);

		*m_skyColour = SKYBOX_COLOUR_DAY;

		if (Shadows::Get() != nullptr)
		{
			Shadows::Get()->GetLightDirection()->Set(lightDirection);
			Shadows::Get()->SetShadowBoxOffset((4.0f * (1.0f - GetShadowFactor())) + 10.0f);
			Shadows::Get()->SetShadowBoxDistance(40.0f);
			Shadows::Get()->SetShadowTransition(5.0f);
			Shadows::Get()->SetShadowDarkness(0.6f * GetShadowFactor());
		}
	}

	float Worlds::GetDayFactor() const
	{
		return m_factorDay;
	}

	float Worlds::GetSunriseFactor() const
	{
		return Maths::Clamp(-(std::sin(2.0f * PI * GetDayFactor()) - 1.0f) / 2.0f, 0.0f, 1.0f);
	}

	float Worlds::GetShadowFactor() const
	{
		return Maths::Clamp(1.7f * std::sin(2.0f * PI * GetDayFactor()), 0.0f, 1.0f);
	}

	float Worlds::GetSunHeight() const
	{
		return m_sunPosition->m_y;
	}

	float Worlds::GetStarIntensity() const
	{
		return Maths::Clamp(1.0f - GetShadowFactor(), 0.0f, 1.0f);
	}
}
