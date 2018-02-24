#include "CelestialBody.hpp"

#include "../Worlds/Worlds.hpp"
#include "../Lights/Light.hpp"

namespace Flounder
{
	CelestialBody::CelestialBody(const CelestialType &type) :
		Component(),
		m_type(type)
	{
		Link<int>(0, "Type", LINK_GET(static_cast<int>(GetType())), LINK_SET(int, SetType(static_cast<CelestialType>(v))));
	}

	CelestialBody::~CelestialBody()
	{
	}

	void CelestialBody::Update()
	{
		Transform *entityTransform = GetGameObject()->GetTransform();

		if (Worlds::Get() != nullptr)
		{
			switch (m_type)
			{
			case CelestialSun:
				entityTransform->SetPosition(*Worlds::Get()->GetSunPosition());
				break;
			case CelestialMoon:
				entityTransform->SetPosition(*Worlds::Get()->GetMoonPosition());
				break;
			}
		}

		auto componentLight = GetGameObject()->GetComponent<Light>();

		if (Worlds::Get() != nullptr && componentLight != nullptr)
		{
			switch (m_type)
			{
			case CelestialSun:
				componentLight->SetColour(*Worlds::Get()->GetSunColour());
				break;
			case CelestialMoon:
				componentLight->SetColour(*Worlds::Get()->GetMoonColour());
				break;
			}
		}
	}
}
