#include "CelestialBody.hpp"

#include "Post/Filters/FilterLensflare.hpp"
#include "Worlds/Worlds.hpp"
#include "Lights/Light.hpp"

namespace fl
{
	CelestialBody::CelestialBody(const CelestialType &type) :
		IComponent(),
		m_type(type)
	{
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
			case CELESTIAL_SUN:
				entityTransform->SetPosition(Worlds::Get()->GetWorld()->GetSunPosition());
				break;
			case CELESTIAL_MOON:
				entityTransform->SetPosition(Worlds::Get()->GetWorld()->GetMoonPosition());
				break;
			}
		}

		auto componentLight = GetGameObject()->GetComponent<Light>();

		if (Worlds::Get() != nullptr && componentLight != nullptr)
		{
			switch (m_type)
			{
			case CELESTIAL_SUN:
				componentLight->SetColour(Worlds::Get()->GetWorld()->GetSunColour());
				break;
			case CELESTIAL_MOON:
				componentLight->SetColour(Worlds::Get()->GetWorld()->GetMoonColour());
				break;
			}
		}

		if (m_type == CELESTIAL_SUN)
		{
			auto filterLensflare = Renderer::Get()->GetManager()->GetRenderer<FilterLensflare>();

			if (filterLensflare != nullptr)
			{
				filterLensflare->SetSunPosition(entityTransform->GetPosition());
				filterLensflare->SetSunHeight(entityTransform->GetPosition().m_y);
			}
		}
	}

	void CelestialBody::Load(LoadedValue *value)
	{
		m_type = static_cast<CelestialType>(value->GetChild("Type")->Get<int>());
	}

	void CelestialBody::Write(LoadedValue *value)
	{
		value->GetChild("Type", true)->Set(static_cast<int>(m_type));
	}
}
