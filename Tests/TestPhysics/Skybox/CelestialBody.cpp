#include "CelestialBody.hpp"

#include <Post/Filters/FilterLensflare.hpp>
#include <Lights/Light.hpp>
#include <Graphics/Graphics.hpp>
#include "World/World.hpp"

namespace test
{
static const Colour SUN_COLOUR_SUNRISE = Colour("#ee9a90");
static const Colour SUN_COLOUR_NIGHT = Colour("#0D0D1A");
static const Colour SUN_COLOUR_DAY = Colour("#ffffff");

static const Colour MOON_COLOUR_NIGHT = Colour("#666699");
static const Colour MOON_COLOUR_DAY = Colour("#000000");

CelestialBody::CelestialBody(const Type &type) :
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
	case Type::Sun:
	{
		Vector3f sunPosition = World::Get()->GetLightDirection() * Vector3f(-6048.0f, -6048.0f, -6048.0f);
		//sunPosition += Scenes::Get()->GetCamera()->GetPosition();
		transform.SetPosition(sunPosition);

		if (componentLight != nullptr)
		{
			Colour sunColour = SUN_COLOUR_SUNRISE.Lerp(SUN_COLOUR_NIGHT, World::Get()->GetSunriseFactor());
			sunColour = sunColour.Lerp(SUN_COLOUR_DAY, World::Get()->GetShadowFactor());
			componentLight->SetColour(sunColour);
		}

		auto filterLensflare = Graphics::Get()->GetSubrender<FilterLensflare>();

		if (filterLensflare != nullptr)
		{
			filterLensflare->SetSunPosition(transform.GetPosition());
			filterLensflare->SetSunHeight(transform.GetPosition().m_y);
		}
	}
		break;
	case Type::Moon:
	{
		Vector3f moonPosition = World::Get()->GetLightDirection() * Vector3f(6048.0f, 6048.0f, 6048.0f);
		//moonPosition += Scenes::Get()->GetCamera()->GetPosition();
		transform.SetPosition(moonPosition);

		if (componentLight != nullptr)
		{
			Colour moonColour = MOON_COLOUR_NIGHT.Lerp(MOON_COLOUR_DAY, World::Get()->GetShadowFactor());
			componentLight->SetColour(moonColour);
		}
	}
		break;
	default:
		break;
	}
}

const Metadata &operator>>(const Metadata &metadata, CelestialBody &celestialBody)
{
	metadata.GetChild("Type", celestialBody.m_type);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const CelestialBody &celestialBody)
{
	metadata.SetChild("Type", celestialBody.m_type);
	return metadata;
}
}
