#include "Force.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
Force::Force(const Vector3& force, const Vector3& position) : m_force(force), m_neverExpires(true), m_position(position) {}

Force::Force(const Vector3& force, const Time& time, const Vector3& position) : m_force(force), m_neverExpires(false), m_timeLeft(time), m_position(position) {}

void Force::Update()
{
	m_timeLeft -= Engine::Get()->GetDelta();
}
}
