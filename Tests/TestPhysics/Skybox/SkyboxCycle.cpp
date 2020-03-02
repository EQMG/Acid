#include "SkyboxCycle.hpp"

#include <Skyboxes/MaterialSkybox.hpp>
#include <Maths/Colour.hpp>
#include <Maths/Transform.hpp>
#include "World/World.hpp"

namespace test {
static const Colour SKYBOX_COLOUR_DAY(0x003C8A);

SkyboxCycle::SkyboxCycle(bool enableFog, bool enableRotation) :
	enableFog(enableFog),
	enableRotation(enableRotation) {
}

void SkyboxCycle::Start() {
}

void SkyboxCycle::Update() {
	auto materialSkybox = GetEntity()->GetComponent<MaterialSkybox>();
	if (!materialSkybox)
		return;

	if (enableFog) {
		materialSkybox->SetBaseColour(SKYBOX_COLOUR_DAY);
		materialSkybox->SetFogColour(World::Get()->GetFog().GetColour());
		materialSkybox->SetFogLimits({World::Get()->GetFog().GetLowerLimit(), World::Get()->GetFog().GetUpperLimit()});
	} else {
		materialSkybox->SetBaseColour(SKYBOX_COLOUR_DAY);
		materialSkybox->SetFogColour(Colour::Black);
		materialSkybox->SetFogLimits(Vector3f(-1000000.0f));
	}

	auto transform = GetEntity()->GetComponent<Transform>();
	if (!transform)
		return;

	if (enableRotation) {
		materialSkybox->SetBlend(World::Get()->GetStarIntensity());
		transform->SetLocalRotation(World::Get()->GetSkyboxRotation());
	} else {
		materialSkybox->SetBlend(1.0f);
		transform->SetLocalRotation({});
	}
}

const Node &operator>>(const Node &node, SkyboxCycle &skyboxCycle) {
	node["enableFog"].Get(skyboxCycle.enableFog);
	node["enableRotation"].Get(skyboxCycle.enableRotation);
	return node;
}

Node &operator<<(Node &node, const SkyboxCycle &skyboxCycle) {
	node["enableFog"].Set(skyboxCycle.enableFog);
	node["enableRotation"].Set(skyboxCycle.enableRotation);
	return node;
}
}
