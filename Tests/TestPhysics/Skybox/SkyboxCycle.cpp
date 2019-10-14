#include "SkyboxCycle.hpp"

#include <Skyboxes/MaterialSkybox.hpp>
#include <Scenes/Entity.inl>
#include <Meshes/Mesh.hpp>
#include <Maths/Colour.hpp>
#include <Maths/Transform.hpp>
#include "World/World.hpp"

namespace test {
bool SkyboxCycle::registered = Register("skyboxCycle");

static const Colour SKYBOX_COLOUR_DAY("#003C8A");

SkyboxCycle::SkyboxCycle(bool enableFog, bool enableRotation) :
	m_enableFog(enableFog),
	m_enableRotation(enableRotation) {
}

void SkyboxCycle::Start() {
}

void SkyboxCycle::Update() {
	auto mesh = GetEntity()->GetComponent<Mesh>();

	if (!mesh) {
		return;
	}

	auto materialSkybox = dynamic_cast<MaterialSkybox *>(mesh->GetMaterial());

	if (!materialSkybox) {
		return;
	}

	if (m_enableFog) {
		materialSkybox->SetBaseColour(SKYBOX_COLOUR_DAY);
		materialSkybox->SetFogColour(World::Get()->GetFog().GetColour());
		materialSkybox->SetFogLimits({World::Get()->GetFog().GetLowerLimit(), World::Get()->GetFog().GetUpperLimit()});
	} else {
		materialSkybox->SetBaseColour(SKYBOX_COLOUR_DAY);
		materialSkybox->SetFogColour(Colour::Black);
		materialSkybox->SetFogLimits(Vector3f(-1000000.0f));
	}

	auto transform = GetEntity()->GetComponent<Transform>();

	if (!transform) {
		return;
	}

	if (m_enableRotation) {
		materialSkybox->SetBlend(World::Get()->GetStarIntensity());
		transform->SetLocalRotation(World::Get()->GetSkyboxRotation());
	} else {
		materialSkybox->SetBlend(1.0f);
		transform->SetLocalRotation({});
	}
}

const Node &operator>>(const Node &node, SkyboxCycle &skyboxCycle) {
	node["enableFog"].Get(skyboxCycle.m_enableFog);
	node["enableRotation"].Get(skyboxCycle.m_enableRotation);
	return node;
}

Node &operator<<(Node &node, const SkyboxCycle &skyboxCycle) {
	node["enableFog"].Set(skyboxCycle.m_enableFog);
	node["enableRotation"].Set(skyboxCycle.m_enableRotation);
	return node;
}
}
