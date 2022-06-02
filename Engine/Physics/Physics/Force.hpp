#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Time.hpp"
#include "Physics/Export.hpp"

namespace acid {
/**
 * @brief Represents a force that is attached to a {@link Rigidbody}.
 */
class ACID_PHYSICS_EXPORT Force {
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

	bool IsExpired() const { return !neverExpires && timeLeft.AsMicroseconds() <= 0.0f; }

	const Vector3f &GetForce() const { return force; }
	void SetForce(const Vector3f &force) { this->force = force; }

	const Time &GetTimeLeft() const { return timeLeft; }
	void SetTimeLeft(const Time &timeLeft) { this->timeLeft = timeLeft; }

	const Vector3f &GetPosition() const { return position; }
	void SetPosition(const Vector3f &position) { this->position = position; }

private:
	Vector3f force;
	bool neverExpires;
	Time timeLeft;
	Vector3f position;
};
}
