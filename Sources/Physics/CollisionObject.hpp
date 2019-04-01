#pragma once

#include "Helpers/Delegate.hpp"
#include "Scenes/Entity.hpp"
#include "Scenes/Component.hpp"
#include "Colliders/Collider.hpp"
#include "Force.hpp"

class btTransform;
class btCollisionShape;
class btCollisionObject;

namespace acid
{
class Frustum;

/**
 * @brief Represents a object in a scene effected by physics.
 */
class ACID_EXPORT CollisionObject :
	public Component
{
public:
	/**
	 * Creates a new collision object.
	 * @param friction The amount of surface friction.
	 */
	explicit CollisionObject(const float &friction = 0.2f);

	virtual ~CollisionObject();

	/**
	 * Gets if the shape is partially in the view frustum.
	 * @param frustum The view frustum.
	 * @return If the shape is partially in the view frustum.
	 */
	virtual bool InFrustum(const Frustum &frustum) = 0;

	Force *AddForce(Force *force);

	template<typename T, typename... Args>
	Force *AddForce(Args &&... args) { return AddForce(new T(std::forward<Args>(args)...)); }

	virtual void ClearForces() = 0;

	bool IsShapeCreated() const { return m_shape != nullptr; }

	void SetChildTransform(Collider *child, const Transform &transform);

	void AddChild(Collider *child);

	void RemoveChild(Collider *child);

	void SetIgnoreCollisionCheck(CollisionObject *other, const bool &ignore);

	const float &GetFriction() const { return m_friction; }

	void SetFriction(const float &friction);

	const float &GetFrictionRolling() const { return m_frictionRolling; }

	void SetFrictionRolling(const float &frictionRolling);

	const float &GetFrictionSpinning() const { return m_frictionSpinning; }

	void SetFrictionSpinning(const float &frictionSpinning);

	/**
	 * Called when this object collides from a object.
	 * @return The delegate.
	 */
	Delegate<void(CollisionObject *)> &OnCollision() { return m_onCollision; }

	/**
	 * Called when this object separates from a object.
	 * @return The delegate.
	 */
	Delegate<void(CollisionObject *)> &OnSeparation() { return m_onSeparation; }

protected:
	void CreateShape(const bool &forceSingle = false);

	virtual void RecalculateMass() = 0;

	float m_friction;
	float m_frictionRolling;
	float m_frictionSpinning;

	std::unique_ptr<btCollisionShape> m_shape;
	btCollisionObject *m_body;

	std::vector<std::unique_ptr<Force>> m_forces;

	Delegate<void(CollisionObject *)> m_onCollision;
	Delegate<void(CollisionObject *)> m_onSeparation;
};
}
