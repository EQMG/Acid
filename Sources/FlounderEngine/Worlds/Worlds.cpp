#include "Worlds.hpp"

#include "Scenes/Scenes.hpp"
#include "Shadows/Shadows.hpp"

namespace fl
{
	const Colour *Worlds::FOG_COLOUR_SUNRISE = new Colour("#ee9a90");
	const Colour *Worlds::FOG_COLOUR_NIGHT = new Colour("#0D0D1A");
	const Colour *Worlds::FOG_COLOUR_DAY = new Colour("#e6e6e6");

	const Colour *Worlds::SUN_COLOUR_SUNRISE = new Colour("#ee9a90");
	const Colour *Worlds::SUN_COLOUR_NIGHT = new Colour("#0D0D1A");
	const Colour *Worlds::SUN_COLOUR_DAY = new Colour("#ffffff");

	const Colour *Worlds::MOON_COLOUR_NIGHT = new Colour("#666699");
	const Colour *Worlds::MOON_COLOUR_DAY = new Colour("#000000");

	const Colour *Worlds::SKYBOX_COLOUR_DAY = new Colour("#003C8A");

	Worlds::Worlds() :
		IModule(),
		m_noiseTerrain(new NoiseFast(69124)),
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
		m_noiseTerrain->SetNoiseType(NoiseFast::PerlinFractal);
		m_noiseTerrain->SetFrequency(0.003f);
		m_noiseTerrain->SetInterp(NoiseFast::Quintic);
		m_noiseTerrain->SetFractalType(NoiseFast::Fbm);
		m_noiseTerrain->SetFractalOctaves(5);
		m_noiseTerrain->SetFractalLacunarity(2.0);
		m_noiseTerrain->SetFractalGain(0.5f);

		m_driverDay->Update(50.0f); // Starts during daytime.
	}

	Worlds::~Worlds()
	{
		delete m_noiseTerrain;

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
		float delta = Engine::Get()->GetDelta();
		m_factorDay = m_driverDay->Update(delta);

		*m_skyboxRotation = Vector3(360.0f * m_factorDay, 0.0f, 0.0f);

		Vector3 lightDirection = m_skyboxRotation->Rotate(Vector3(0.2f, 0.0f, 0.5f));
		lightDirection.Normalize();

		Colour fogColour = FOG_COLOUR_SUNRISE->Interpolate(*FOG_COLOUR_NIGHT, GetSunriseFactor());
		fogColour = fogColour.Interpolate(*FOG_COLOUR_DAY, GetShadowFactor());

		*m_sunPosition = lightDirection * Vector3(-6048.0f, -6048.0f, -6048.0f);
		*m_moonPosition = lightDirection * Vector3(6048.0f, 6048.0f, 6048.0f);

		/*if (Scenes::Get()->GetCamera() != nullptr)
		{
			Vector3::Add(*m_sunPosition, *Scenes::Get()->GetCamera()->GetPosition(), m_sunPosition);
			Vector3::Add(*m_moonPosition, *Scenes::Get()->GetCamera()->GetPosition(), m_moonPosition);
		}*/

		*m_sunColour = SUN_COLOUR_SUNRISE->Interpolate(*SUN_COLOUR_NIGHT, GetSunriseFactor());
		*m_sunColour = m_sunColour->Interpolate(*SUN_COLOUR_DAY, GetShadowFactor());

		*m_moonColour = MOON_COLOUR_NIGHT->Interpolate(*MOON_COLOUR_DAY, GetShadowFactor());

		*m_fog->m_colour = fogColour;
		m_fog->m_density = 0.002f + ((1.0f - GetShadowFactor()) * 0.002f);
		m_fog->m_gradient = 2.0f - ((1.0f - GetShadowFactor()) * 0.380f);
		m_fog->m_lowerLimit = 0.0f;
		m_fog->m_upperLimit = 0.15f - ((1.0f - GetShadowFactor()) * 0.03f);

		*m_skyColour = *SKYBOX_COLOUR_DAY;

		if (Shadows::Get() != nullptr)
		{
			Shadows::Get()->SetLightDirection(lightDirection);
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

	float Worlds::GetTerrainRadius(const float &radius, const float &theta, const float &phi)
	{
		float height = m_noiseTerrain->GetValue(
			(radius / 10.0f) * Maths::NormalizeAngle(Maths::Degrees(theta)),
			(radius / 10.0f) * Maths::NormalizeAngle(Maths::Degrees(phi))
		);
		return radius + (28.0f * height);
	}
}
