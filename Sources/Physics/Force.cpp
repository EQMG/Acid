#include "Force.hpp"

namespace acid {
Force::Force(const Vector3f &force, const Vector3f &position) :
	m_force(force),
	m_neverExpires(true),
	m_position(position) {
}

Force::Force(const Vector3f &force, const Time &time, const Vector3f &position) :
	m_force(force),
	m_neverExpires(false),
	m_timeLeft(time),
	m_position(position) {
}

void Force::Update(const Time &delta) {
	m_timeLeft -= delta;
}
}
