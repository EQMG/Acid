#pragma once

#include "Maths/Vector3.hpp"

namespace acid
{
	class ACID_EXPORT Force
	{
	private:
		Vector3 m_force;
		bool m_neverExpires;
		float m_timeLeft;
		Vector3 m_position;
	public:
		Force(const Vector3 &force, const Vector3 &position = Vector3::ZERO);

		Force(const Vector3 &force, const float &time, const Vector3 &position = Vector3::ZERO);

		void Update();

		bool IsExpired() const { return !m_neverExpires && m_timeLeft <= 0.0f; }

		Vector3 GetForce() const { return m_force; }

		void SetForce(const Vector3 &force) { m_force = force; }

		float GetTimeLeft() const { return m_timeLeft; }

		void SetTimeLeft(float timeLeft) { m_timeLeft = timeLeft; }

		Vector3 GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) { m_position = position; }
	};
}
