#include "Force.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
	Force::Force(const Vector3 &direction, const Vector3 &position) :
		m_direction(direction),
		m_neverExpires(true),
		m_timeLeft(Time::ZERO),
		m_position(position)
	{
	}

	Force::Force(const Vector3 &direction, const Time &time, const Vector3 &position) :
		m_direction(direction),
		m_neverExpires(false),
		m_timeLeft(time),
		m_position(position)
	{
	}

	void Force::Update()
	{
		m_timeLeft -= Engine::Get()->GetDelta();
	}
}
