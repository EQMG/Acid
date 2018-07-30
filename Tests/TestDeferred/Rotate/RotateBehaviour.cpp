#include "RotateBehaviour.hpp"

#include <Engine/Engine.hpp>

namespace test
{
	RotateBehaviour::RotateBehaviour(const Vector3 &rate) :
		IBehaviour(),
		m_rate(rate),
		m_rotation(Vector3())
	{
	}

	RotateBehaviour::~RotateBehaviour()
	{
	}

	void RotateBehaviour::Start()
	{
		m_rotation = GetGameObject()->GetTransform().GetRotation().ToEuler();
	}

	void RotateBehaviour::Update()
	{
		m_rotation += Engine::Get()->GetDelta() * m_rate;
		GetGameObject()->GetTransform().SetRotation(m_rotation.ToQuaternion());
	}

	void RotateBehaviour::Load(LoadedValue *value)
	{
		m_rate = value->GetChild("Rate");
	}

	void RotateBehaviour::Write(LoadedValue *destination)
	{
		m_rate.Write(destination->GetChild("Rate", true));
	}
}
