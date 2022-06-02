#include "CelestialBody.hpp"

#include <Post/Filters/LensflareFilter.hpp>
#include <Lights/Light.hpp>
#include <Graphics/Graphics.hpp>
#include "World/World.hpp"
#include "Maths/Transform.hpp"

namespace test {
constexpr static Colour SUN_COLOUR_SUNRISE(0xEE9A90);
constexpr static Colour SUN_COLOUR_NIGHT(0x0D0D1A);
constexpr static Colour SUN_COLOUR_DAY(0xFFFFFF);
			
constexpr static Colour MOON_COLOUR_NIGHT(0x666699);
constexpr static Colour MOON_COLOUR_DAY(0x000000);

CelestialBody::CelestialBody(Type type) :
	type(type) {
}

void CelestialBody::Start() {
}

void CelestialBody::Update() {
	auto transform = GetEntity()->GetComponent<Transform>();
	if (!transform)
		return;

	auto world = Scenes::Get()->GetScene()->GetSystem<World>();
	if (!world)
		return;
	//auto camera = Scenes::Get()->GetScene()->GetCamera();

	switch (type) {
	case Type::Sun: {
		auto sunPosition = world->GetLightDirection() * Vector3f(-6048.0f, -6048.0f, -6048.0f);
		//sunPosition += camera->GetPosition();
		transform->SetLocalPosition(sunPosition);

		if (auto light = GetEntity()->GetComponent<Light>()) {
			auto sunColour = SUN_COLOUR_SUNRISE.Lerp(SUN_COLOUR_NIGHT, world->GetSunriseFactor());
			sunColour = sunColour.Lerp(SUN_COLOUR_DAY, world->GetShadowFactor());
			light->SetColour(sunColour);
		}

		if (auto filterLensflare = Graphics::Get()->GetRenderer()->GetSubrender<LensflareFilter>()) {
			filterLensflare->SetSunPosition(transform->GetPosition());
			filterLensflare->SetSunHeight(transform->GetPosition().y);
		}
	}
	break;
	case Type::Moon: {
		auto moonPosition = world->GetLightDirection() * Vector3f(6048.0f, 6048.0f, 6048.0f);
		//moonPosition += camera->GetPosition();
		transform->SetLocalPosition(moonPosition);

		if (auto light = GetEntity()->GetComponent<Light>()) {
			auto moonColour = MOON_COLOUR_NIGHT.Lerp(MOON_COLOUR_DAY, world->GetShadowFactor());
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
