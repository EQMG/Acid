#include "ComponentSun.hpp"

#include "../../Worlds/Worlds.hpp"
#include "../Entity.hpp"
#include "ComponentLight.hpp"

namespace Flounder
{
	ComponentSun::ComponentSun() :
		IComponent()
	{
	}

	ComponentSun::~ComponentSun()
	{
	}

	void ComponentSun::Update()
	{
		Transform *entityTransform = GetEntity()->GetTransform();

		if (Worlds::Get() != nullptr)
		{
			entityTransform->SetPosition(*Worlds::Get()->GetSunPosition());
		}

		auto componentLight = GetEntity()->GetComponent<ComponentLight>();

		if (Worlds::Get() != nullptr && componentLight != nullptr)
		{
			componentLight->GetLight()->m_colour->Set(*Worlds::Get()->GetSunColour());
		}
	}

	void ComponentSun::CmdRender(EntityRender *entityRender)
	{
		// TODO: Disable fog/shadows/lighting.
	}
}
