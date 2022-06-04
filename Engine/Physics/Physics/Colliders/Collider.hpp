#pragma once

#include <rocket.hpp>

#include "Maths/Transform.hpp"
#include "Maths/Quaternion.hpp"
#include "Utils/StreamFactory.hpp"
#include "PhysicsExport.hpp"

class btCollisionShape;
class btVector3;
class btQuaternion;
class btTransform;

namespace acid {
/**
 * @brief Class that represents a physics shape.
 */
class ACID_PHYSICS_EXPORT Collider : public StreamFactory<Collider> {
public:
	/**
	 * Creates a new collider.
	 * @param localTransform The parent offset of the body.
	 */
	explicit Collider(const Transform &localTransform = {});
	virtual ~Collider() = default;

	/**
	 * Gets the collision shape defined in this collider.
	 * @return The collision shape.
	 */
	virtual btCollisionShape *GetCollisionShape() const = 0;

	const Transform &GetLocalTransform() const { return localTransform; }
	void SetLocalTransform(const Transform &localTransform);

	rocket::signal<void(Collider *, const Transform &)> &OnTransformChange() { return onTransformChange; }

	static btVector3 Convert(const Vector3f &vector);
	static Vector3f Convert(const btVector3 &vector);
	static btQuaternion Convert(const Quaternion &quaternion);
	static Quaternion Convert(const btQuaternion &quaternion);
	static btTransform Convert(const Transform &transform);
	static Transform Convert(const btTransform &transform, const Vector3f &scaling = Vector3f(1.0f));

protected:
	Transform localTransform;
	rocket::signal<void(Collider*, const Transform &)> onTransformChange;
};
}
