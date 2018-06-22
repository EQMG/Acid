#include "SkyboxCycle.hpp"

#include <Skyboxes/MaterialSkybox.hpp>
#include <Maths/Colour.hpp>
#include "World/World.hpp"

namespace test
{
	static const Colour SKYBOX_COLOUR_DAY = Colour("#003C8A");

	SkyboxCycle::SkyboxCycle(const bool &enableFog) :
		IComponent(),
		m_enableFog(enableFog)
	{
	}

	SkyboxCycle::~SkyboxCycle()
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
			Vector3 skyboxRotation = World::Get()->GetSkyboxRotation();
			GetGameObject()->GetTransform().SetRotation(skyboxRotation);
			materialSkybox->SetSkyColour(SKYBOX_COLOUR_DAY);
			materialSkybox->SetBlend(World::Get()->GetStarIntensity());
			materialSkybox->SetFogColour(World::Get()->GetFog().GetColour());
			materialSkybox->SetFogLimits(Vector2(World::Get()->GetFog().GetLowerLimit(), World::Get()->GetFog().GetUpperLimit()));
			return;
		}

		GetGameObject()->GetTransform().SetRotation(Vector3::ZERO);
		materialSkybox->SetSkyColour(SKYBOX_COLOUR_DAY);
		materialSkybox->SetBlend(1.0f);
		materialSkybox->SetFogColour(Colour::BLACK);
		materialSkybox->SetFogLimits(Vector2(-1000000.0f, -1000000.0f));
	}

	void SkyboxCycle::Load(LoadedValue *value)
	{
		m_enableFog = value->GetChild("Enable Fog")->Get<bool>();
	}

	void SkyboxCycle::Write(LoadedValue *destination)
	{
		destination->GetChild("Enable Fog", true)->Set((int) m_enableFog);
	}
}
