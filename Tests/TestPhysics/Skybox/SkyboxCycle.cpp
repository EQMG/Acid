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
		auto materialSkybox = GetGameObject()->GetComponent<MaterialSkybox>();

		if (materialSkybox == nullptr)
		{
			return;
		}

		if (m_enableFog)
		{
			materialSkybox->SetSkyColour(SKYBOX_COLOUR_DAY);
			materialSkybox->SetFogColour(World::Get()->GetFog().GetColour());
			materialSkybox->SetFogLimits(Vector2(World::Get()->GetFog().GetLowerLimit(), World::Get()->GetFog().GetUpperLimit()));
		}
		else
		{
			materialSkybox->SetSkyColour(SKYBOX_COLOUR_DAY);
			materialSkybox->SetFogColour(Colour::BLACK);
			materialSkybox->SetFogLimits(Vector2(-1000000.0f, -1000000.0f));
		}

		if (m_enableRotation)
		{
			materialSkybox->SetBlend(World::Get()->GetStarIntensity());
			GetGameObject()->GetLocalTransform().SetRotation(World::Get()->GetSkyboxRotation());
		}
		else
		{
			materialSkybox->SetBlend(1.0f);
			GetGameObject()->GetLocalTransform().SetRotation(Vector3::ZERO);
		}
	}

	void SkyboxCycle::Decode(const Metadata &metadata)
	{
		m_enableFog = metadata.GetChild<bool>("Enable Fog");
		m_enableRotation = metadata.GetChild<bool>("Enable Rotation");
	}

	void SkyboxCycle::Encode(Metadata &metadata) const
	{
		metadata.SetChild<bool>("Enable Fog", m_enableFog);
		metadata.SetChild<bool>("Enable Rotation", m_enableRotation);
	}
}
