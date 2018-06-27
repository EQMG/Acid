#include "Rigidbody.hpp"

#include "Models/Model.hpp"
#include "Scenes/Scenes.hpp"

namespace fl
{
	Rigidbody::Rigidbody(const float &mass, const float &drag, const bool &useGravity, const Constraint3 &freezePosition, const Constraint3 &freezeRotation) :
		IComponent(),
		m_mass(mass),
		m_drag(drag),
		m_useGravity(useGravity),
		m_freezePosition(freezePosition),
		m_freezeRotation(freezeRotation)
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::Update()
	{
	}

	void Rigidbody::Load(LoadedValue *value)
	{
		m_mass = value->GetChild("Mass")->Get<float>();
		m_drag = value->GetChild("Drag")->Get<float>();
		m_useGravity = value->GetChild("Use Gravity")->Get<bool>();
		m_freezePosition = value->GetChild("Freeze Position");
		m_freezeRotation = value->GetChild("Freeze Rotation");
	}

	void Rigidbody::Write(LoadedValue *destination)
	{
		destination->GetChild("Mass", true)->Set(m_mass);
		destination->GetChild("Drag", true)->Set(m_drag);
		destination->GetChild("Use Gravity", true)->Set(m_useGravity);
		m_freezePosition.Write(destination->GetChild("Freeze Position", true));
		m_freezeRotation.Write(destination->GetChild("Freeze Rotation", true));
	}
}
