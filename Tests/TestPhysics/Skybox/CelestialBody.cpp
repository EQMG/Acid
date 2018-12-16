#include "CelestialBody.hpp"

#include <Post/Filters/FilterLensflare.hpp>
#include <Lights/Light.hpp>
#include <Renderer/Renderer.hpp>
#include "World/World.hpp"

namespace test
{
	static const Colour SUN_COLOUR_SUNRISE = Colour("#ee9a90");
	static const Colour SUN_COLOUR_NIGHT = Colour("#0D0D1A");
	static const Colour SUN_COLOUR_DAY = Colour("#ffffff");

	static const Colour MOON_COLOUR_NIGHT = Colour("#666699");
	static const Colour MOON_COLOUR_DAY = Colour("#000000");

	CelestialBody::CelestialBody(const CelestialType &type) :
		m_type(type)
	{
	}

	void CelestialBody::Start()
	{
	}

	void CelestialBody::Update()
	{
		auto &transform = GetParent()->GetLocalTransform();
		auto componentLight = GetParent()->GetComponent<Light>();

		switch (m_type)
		{
		case CELESTIAL_SUN:
		{
			Vector3 sunPosition = World::Get()->GetLightDirection() * Vector3(-6048.0f, -6048.0f, -6048.0f);
		//	sunPosition += Scenes::Get()->GetCamera()->GetPosition();
			transform.SetPosition(sunPosition);

			if (componentLight != nullptr)
			{
				Colour sunColour = SUN_COLOUR_SUNRISE.Interpolate(SUN_COLOUR_NIGHT, World::Get()->GetSunriseFactor());
				sunColour = sunColour.Interpolate(SUN_COLOUR_DAY, World::Get()->GetShadowFactor());
				componentLight->SetColour(sunColour);
			}

			auto filterLensflare = Renderer::Get()->GetRendererRegister().Get<FilterLensflare>();

			if (filterLensflare != nullptr)
			{
				filterLensflare->SetSunPosition(transform.GetPosition());
				filterLensflare->SetSunHeight(transform.GetPosition().m_y);
			}
		}
			break;
		case CELESTIAL_MOON:
		{
			Vector3 moonPosition = World::Get()->GetLightDirection() * Vector3(6048.0f, 6048.0f, 6048.0f);
		//	moonPosition += Scenes::Get()->GetCamera()->GetPosition();
			transform.SetPosition(moonPosition);

			if (componentLight != nullptr)
			{
				Colour moonColour = MOON_COLOUR_NIGHT.Interpolate(MOON_COLOUR_DAY, World::Get()->GetShadowFactor());
				componentLight->SetColour(moonColour);
			}
		}
			break;
		default:
			break;
		}
	}

	void CelestialBody::Decode(const Metadata &metadata)
	{
	//	m_type = metadata.GetChild<CelestialType>("Type"); // TODO: Node child enum type.
	}

	void CelestialBody::Encode(Metadata &metadata) const
	{
	//	metadata.SetChild<CelestialType>("Type", m_type);
	}
}
