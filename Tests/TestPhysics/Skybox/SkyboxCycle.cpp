#include "SkyboxCycle.hpp"

#include <Skyboxes/MaterialSkybox.hpp>
#include <Maths/Colour.hpp>
#include "World/World.hpp"

namespace test
{
static const Colour SKYBOX_COLOUR_DAY = Colour("#003C8A");

SkyboxCycle::SkyboxCycle(const bool &enableFog, const bool &enableRotation) :
	m_enableFog(enableFog),
	m_enableRotation(enableRotation)
{
}

void SkyboxCycle::Start()
{
}

void SkyboxCycle::Update()
{
	auto materialSkybox = GetParent()->GetComponent<MaterialSkybox>();

	if (materialSkybox == nullptr)
	{
		return;
	}

	if (m_enableFog)
	{
		materialSkybox->SetBaseColour(SKYBOX_COLOUR_DAY);
		materialSkybox->SetFogColour(World::Get()->GetFog().GetColour());
		materialSkybox->SetFogLimits(Vector2f(World::Get()->GetFog().GetLowerLimit(), World::Get()->GetFog().GetUpperLimit()));
	}
	else
	{
		materialSkybox->SetBaseColour(SKYBOX_COLOUR_DAY);
		materialSkybox->SetFogColour(Colour::Black);
		materialSkybox->SetFogLimits(Vector2f(-1000000.0f, -1000000.0f));
	}

	if (m_enableRotation)
	{
		materialSkybox->SetBlend(World::Get()->GetStarIntensity());
		GetParent()->GetLocalTransform().SetRotation(World::Get()->GetSkyboxRotation());
	}
	else
	{
		materialSkybox->SetBlend(1.0f);
		GetParent()->GetLocalTransform().SetRotation(Vector3f());
	}
}

const Metadata &operator>>(const Metadata &metadata, SkyboxCycle &skyboxCycle)
{
	metadata.GetChild("Enable Fog", skyboxCycle.m_enableFog);
	metadata.GetChild("Enable Rotation", skyboxCycle.m_enableRotation);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const SkyboxCycle &skyboxCycle)
{
	metadata.SetChild("Enable Fog", skyboxCycle.m_enableFog);
	metadata.SetChild("Enable Rotation", skyboxCycle.m_enableRotation);
	return metadata;
}
}
