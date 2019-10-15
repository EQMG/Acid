#include "CelestialBody.hpp"

#include <Post/Filters/FilterLensflare.hpp>
#include <Lights/Light.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Entity.inl>
#include "World/World.hpp"
#include "Maths/Transform.hpp"

namespace test {
bool CelestialBody::registered = Register("celestialBody");

static const Colour SUN_COLOUR_SUNRISE("#ee9a90");
static const Colour SUN_COLOUR_NIGHT("#0D0D1A");
static const Colour SUN_COLOUR_DAY("#ffffff");

static const Colour MOON_COLOUR_NIGHT("#666699");
static const Colour MOON_COLOUR_DAY("#000000");

CelestialBody::CelestialBody(Type type) :
	m_type(type) {
}

void CelestialBody::Update() {
	auto transform = GetEntity()->GetComponent<Transform>();

	if (!transform) {
		return;
	}

	switch (m_type) {
	case Type::Sun: {
		auto sunPosition = World::Get()->GetLightDirection() * Vector3f(-6048.0f, -6048.0f, -6048.0f);
		//sunPosition += Scenes::Get()->GetCamera()->GetPosition();
		transform->SetLocalPosition(sunPosition);

		if (auto light = GetEntity()->GetComponent<Light>()) {
			auto sunColour = SUN_COLOUR_SUNRISE.Lerp(SUN_COLOUR_NIGHT, World::Get()->GetSunriseFactor());
			sunColour = sunColour.Lerp(SUN_COLOUR_DAY, World::Get()->GetShadowFactor());
			light->SetColour(sunColour);
		}

		if (auto filterLensflare = Graphics::Get()->GetRenderer()->GetSubrender<FilterLensflare>()) {
			filterLensflare->SetSunPosition(transform->GetPosition());
			filterLensflare->SetSunHeight(transform->GetPosition().m_y);
		}
	}
	break;
	case Type::Moon: {
		auto moonPosition = World::Get()->GetLightDirection() * Vector3f(6048.0f, 6048.0f, 6048.0f);
		//moonPosition += Scenes::Get()->GetCamera()->GetPosition();
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
	node["type"].Get(celestialBody.m_type);
	return node;
}

Node &operator<<(Node &node, const CelestialBody &celestialBody) {
	node["type"].Set(celestialBody.m_type);
	return node;
}
}
