#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Time.hpp"

namespace acid
{
/// <summary>
/// Represents a force that is attached to a <seealso cref="Rigidbody"/>.
/// </summary>
class ACID_EXPORT Force
{
public:
	/// <summary>
	/// Creates a new force that never times-out.
	/// </summary>
	/// <param name="force"> The vector of the force is applied. </param>
	/// <param name="position"> The position offset from the centre of the shape to apply the force. </param>
	Force(const Vector3 &force, const Vector3 &position = Vector3::Zero);

	/// <summary>
	/// Creates a new force that times-out.
	/// </summary>
	/// <param name="force"> The vector of the force is applied. </param>
	/// <param name="time"> How long the force will be applied for. </param>
	/// <param name="position"> The position offset from the centre of the shape to apply the force. </param>
	Force(const Vector3 &force, const Time &time, const Vector3 &position = Vector3::Zero);

	void Update();

	bool IsExpired() const { return !m_neverExpires && m_timeLeft.AsMicroseconds() <= 0.0f; }

	const Vector3 &GetForce() const { return m_force; }

	void SetForce(const Vector3 &force) { m_force = force; }

	const Time &GetTimeLeft() const { return m_timeLeft; }

	void SetTimeLeft(const Time &timeLeft) { m_timeLeft = timeLeft; }

	const Vector3 &GetPosition() const { return m_position; }

	void SetPosition(const Vector3 &position) { m_position = position; }

private:
	Vector3 m_force;
	bool m_neverExpires;
	Time m_timeLeft;
	Vector3 m_position;
};
}
