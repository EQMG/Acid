#include "RotateBehaviour.hpp"

#include <Engine/Engine.hpp>

namespace test
{
	RotateBehaviour::RotateBehaviour(const Vector3 &rate) :
		IBehaviour(),
		m_rate(rate)
	{
	}

	RotateBehaviour::~RotateBehaviour()
	{
	}

	void RotateBehaviour::Start()
	{
	}

	void RotateBehaviour::Update()
	{
		Vector3 euler = GetGameObject()->GetTransform().GetRotation().ToEuler();
		euler += Engine::Get()->GetDelta() * m_rate;
		GetGameObject()->GetTransform().SetRotation(euler.ToQuaternion());
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
