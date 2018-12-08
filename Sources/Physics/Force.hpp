#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Time.hpp"

namespace acid
{
	class ACID_EXPORT Force
	{
	private:
		Vector3 m_direction;
		bool m_neverExpires;
		Time m_timeLeft;
		Vector3 m_position;
	public:
		Force(const Vector3 &direction, const Vector3 &position = Vector3::ZERO);

		Force(const Vector3 &direction, const Time &time, const Vector3 &position = Vector3::ZERO);

		void Update();

		bool IsExpired() const { return !m_neverExpires && m_timeLeft.AsMicroseconds() <= 0.0f; }

		Vector3 GetDirection() const { return m_direction; }

		void SetDirection(const Vector3 &direction) { m_direction = direction; }

		Time GetTimeLeft() const { return m_timeLeft; }

		void SetTimeLeft(const Time &timeLeft) { m_timeLeft = timeLeft; }

		Vector3 GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) { m_position = position; }
	};
}
