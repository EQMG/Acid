#include "Force.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
Force::Force(const Vector3f &force, const Vector3f &position) :
	m_force{force},
	m_neverExpires{true},
	m_position{position}
{
}

Force::Force(const Vector3f &force, const Time &time, const Vector3f &position) :
	m_force{force},
	m_neverExpires{false},
	m_timeLeft{time},
	m_position{position}
{
}

void Force::Update()
{
	m_timeLeft -= Engine::Get()->GetDelta();
}
}
