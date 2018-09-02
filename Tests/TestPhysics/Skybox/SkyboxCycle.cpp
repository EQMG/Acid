#include "SkyboxCycle.hpp"

#include <Skyboxes/MaterialSkybox.hpp>
#include <Maths/Colour.hpp>
#include "World/World.hpp"

namespace test
{
	static const Colour SKYBOX_COLOUR_DAY = Colour("#003C8A");

	SkyboxCycle::SkyboxCycle(const bool &enableFog, const bool &enableRotation) :
		IComponent(),
		m_enableFog(enableFog),
		m_enableRotation(enableRotation)
	{
	}

	SkyboxCycle::~SkyboxCycle()
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
			GetGameObject()->GetTransform().SetRotation(World::Get()->GetSkyboxRotation());
		}
		else
		{
			materialSkybox->SetBlend(1.0f);
			GetGameObject()->GetTransform().SetRotation(Vector3::ZERO);
		}
	}

	void SkyboxCycle::Load(LoadedValue &value)
	{
		m_enableFog = value.FindChild("Enable Fog")->Get<bool>();
		m_enableRotation = value.FindChild("Enable Rotation")->Get<bool>();
	}

	void SkyboxCycle::Write(LoadedValue &destination)
	{
		destination.FindChild("Enable Fog", true)->Set(static_cast<int>(m_enableFog));
		destination.FindChild("Enable Rotation", true)->Set(static_cast<int>(m_enableRotation));
	}
}
