#include "SkyboxCycle.hpp"

#include <Skyboxes/MaterialSkybox.hpp>
#include <Maths/Colour.hpp>
#include <Maths/Transform.hpp>
#include "World/World.hpp"

namespace test
{
static const Colour SKYBOX_COLOUR_DAY{"#003C8A"};

SkyboxCycle::SkyboxCycle(const bool &enableFog, const bool &enableRotation) :
	m_enableFog{enableFog},
	m_enableRotation{enableRotation}
{
}

void SkyboxCycle::Start()
{
}

void SkyboxCycle::Update()
{
	auto materialSkybox{GetEntity()->GetComponent<MaterialSkybox>()};

	if (materialSkybox == nullptr)
	{
		return;
	}

	if (m_enableFog)
	{
		materialSkybox->SetBaseColour(SKYBOX_COLOUR_DAY);
		materialSkybox->SetFogColour(World::Get()->GetFog().GetColour());
		materialSkybox->SetFogLimits({World::Get()->GetFog().GetLowerLimit(), World::Get()->GetFog().GetUpperLimit()});
	}
	else
	{
		materialSkybox->SetBaseColour(SKYBOX_COLOUR_DAY);
		materialSkybox->SetFogColour(Colour::Black);
		materialSkybox->SetFogLimits({-1000000.0f});
	}

	auto transform{GetEntity()->GetComponent<Transform>()};

	if (transform == nullptr)
	{
		return;
	}

	if (m_enableRotation)
	{
		materialSkybox->SetBlend(World::Get()->GetStarIntensity());
		transform->SetLocalRotation(World::Get()->GetSkyboxRotation());
	}
	else
	{
		materialSkybox->SetBlend(1.0f);
		transform->SetLocalRotation({});
	}
}

const Metadata &operator>>(const Metadata &metadata, SkyboxCycle &skyboxCycle)
{
	metadata.GetChild("enableFog", skyboxCycle.m_enableFog);
	metadata.GetChild("enableRotation", skyboxCycle.m_enableRotation);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const SkyboxCycle &skyboxCycle)
{
	metadata.SetChild("enableFog", skyboxCycle.m_enableFog);
	metadata.SetChild("enableRotation", skyboxCycle.m_enableRotation);
	return metadata;
}
}
