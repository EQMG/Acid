#pragma once

#include "Maths/Transform.hpp"
#include "Gizmos/Gizmos.hpp"
#include "Scenes/Component.hpp"

class btCollisionShape;
class btVector3;
class btQuaternion;
class btTransform;

namespace acid
{
/**
 * @brief Class that represents a physics shape.
 */
class ACID_EXPORT Collider :
	public Component
{
public:
	/**
	 * Creates a new collider.
	 * @param localTransform The parent offset of the body.
	 * @param gizmoType The gizmo type to use for this collider type.
	 */
	explicit Collider(const Transform &localTransform = Transform(), const std::shared_ptr<GizmoType> &gizmoType = nullptr);

	virtual ~Collider();

	void Update() override;

	/**
	 * Gets the collision shape defined in this collider.
	 * @return The collision shape.
	 */
	virtual btCollisionShape *GetCollisionShape() const = 0;

	const Transform &GetLocalTransform() const { return m_localTransform; }

	void SetLocalTransform(const Transform &localTransform);

	static btVector3 Convert(const Vector3f &vector);

	static Vector3f Convert(const btVector3 &vector);

	static btQuaternion Convert(const Quaternion &quaternion);

	static Quaternion Convert(const btQuaternion &quaternion);

	static btTransform Convert(const Transform &transform);

	static Transform Convert(const btTransform &transform, const Vector3f &scaling = {1.0f});

protected:
	Transform m_localTransform;
	Gizmo *m_gizmo{};
};
}
