#include <Engine/Engine.hpp>
#include "Planet.hpp"

namespace Demo
{
	Planet::Planet(const float &apoapsis) :
		Component(),
		m_apoapsis(apoapsis)
	{
	}

	Planet::~Planet()
	{
	}

	void Planet::Update()
	{
		Vector3::Rotate(Vector3(m_apoapsis, 0.0f, 0.0f), Vector3(0.0f, Engine::Get()->GetTime(), 0.0f), GetGameObject()->GetTransform()->GetPosition());
	}

	void Planet::Load(LoadedValue *value)
	{
		m_apoapsis = value->GetChild("Apoapsis")->Get<float>();
	}

	void Planet::Write(LoadedValue *value)
	{
		value->GetChild("Apoapsis", true)->Set(m_apoapsis);
	}
}
