#include "CelestialBody.hpp"

#include <Post/Filters/LensflareFilter.hpp>
#include <Lights/Light.hpp>
#include <Graphics/Graphics.hpp>
#include "World/World.hpp"
#include "Maths/Transform.hpp"

namespace test {
static constexpr Colour SUN_COLOUR_SUNRISE(0xEE9A90);
static constexpr Colour SUN_COLOUR_NIGHT(0x0D0D1A);
static constexpr Colour SUN_COLOUR_DAY(0xFFFFFF);
			
static constexpr Colour MOON_COLOUR_NIGHT(0x666699);
static constexpr Colour MOON_COLOUR_DAY(0x000000);

CelestialBody::CelestialBody(Type type) :
	type(type) {
}

void CelestialBody::Start() {
}

void CelestialBody::Update() {
	auto transform = GetEntity()->GetComponent<Transform>();
	if (!transform)
		return;

	switch (type) {
	case Type::Sun: {
		auto sunPosition = World::Get()->GetLightDirection() * Vector3f(-6048.0f, -6048.0f, -6048.0f);
		//sunPosition += Scenes::Get()->GetScene()->GetCamera()->GetPosition();
		transform->SetLocalPosition(sunPosition);

		if (auto light = GetEntity()->GetComponent<Light>()) {
			auto sunColour = SUN_COLOUR_SUNRISE.Lerp(SUN_COLOUR_NIGHT, World::Get()->GetSunriseFactor());
			sunColour = sunColour.Lerp(SUN_COLOUR_DAY, World::Get()->GetShadowFactor());
			light->SetColour(sunColour);
		}

		if (auto filterLensflare = Graphics::Get()->GetRenderer()->GetSubrender<LensflareFilter>()) {
			filterLensflare->SetSunPosition(transform->GetPosition());
			filterLensflare->SetSunHeight(transform->GetPosition().y);
		}
	}
	break;
	case Type::Moon: {
		auto moonPosition = World::Get()->GetLightDirection() * Vector3f(6048.0f, 6048.0f, 6048.0f);
		//moonPosition += Scenes::Get()->GetScene()->GetCamera()->GetPosition();
		transform->SetLocalPosition(moonPosition);

		if (auto light = GetEntity()->GetComponent<Light>()) {
			auto moonColour = MOON_COLOUR_NIGHT.Lerp(MOON_COLOUR_DAY, World::Get()->GetShadowFactor());
			light->SetColour(moonColour);
		}
	}
	break;
	default:
		break;
	}
}

const Node &operator>>(const Node &node, CelestialBody &celestialBody) {
	node["type"].Get(celestialBody.type);
	return node;
}

Node &operator<<(Node &node, const CelestialBody &celestialBody) {
	node["type"].Set(celestialBody.type);
	return node;
}
}
