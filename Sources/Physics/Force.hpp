#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Time.hpp"

namespace acid
{
	class ACID_EXPORT Force
	{
	private:
		Vector3 m_force;
		bool m_neverExpires;
		Time m_timeLeft;
		Vector3 m_position;
	public:
		Force(const Vector3 &force, const Vector3 &position = Vector3::ZERO);

		Force(const Vector3 &force, const Time &time, const Vector3 &position = Vector3::ZERO);

		void Update();

		bool IsExpired() const { return !m_neverExpires && m_timeLeft.AsMicroseconds() <= 0.0f; }

		Vector3 GetForce() const { return m_force; }

		void SetForce(const Vector3 &force) { m_force = force; }

		Time GetTimeLeft() const { return m_timeLeft; }

		void SetTimeLeft(const Time &timeLeft) { m_timeLeft = timeLeft; }

		Vector3 GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) { m_position = position; }
	};
}
