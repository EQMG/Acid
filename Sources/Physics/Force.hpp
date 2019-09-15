#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Time.hpp"

namespace acid {
/**
 * @brief Represents a force that is attached to a {@link Rigidbody}.
 */
class ACID_EXPORT Force {
public:
	/**
	 * Creates a new force that never times-out.
	 * @param force The vector of the force is applied.
	 * @param position The position offset from the centre of the shape to apply the force.
	 */
	explicit Force(const Vector3f &force, const Vector3f &position = {});

	/**
	 * Creates a new force that times-out.
	 * @param force The vector of the force is applied.
	 * @param time How long the force will be applied for.
	 * @param position The position offset from the centre of the shape to apply the force.
	 */
	Force(const Vector3f &force, const Time &time, const Vector3f &position = {});

	void Update(const Time &delta);

	bool IsExpired() const { return !m_neverExpires && m_timeLeft.AsMicroseconds() <= 0.0f; }

	const Vector3f &GetForce() const { return m_force; }
	void SetForce(const Vector3f &force) { m_force = force; }

	const Time &GetTimeLeft() const { return m_timeLeft; }
	void SetTimeLeft(const Time &timeLeft) { m_timeLeft = timeLeft; }

	const Vector3f &GetPosition() const { return m_position; }
	void SetPosition(const Vector3f &position) { m_position = position; }

private:
	Vector3f m_force;
	bool m_neverExpires;
	Time m_timeLeft;
	Vector3f m_position;
};
}
