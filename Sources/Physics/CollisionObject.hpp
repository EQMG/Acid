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
	 * @param mass The mass of the object.
	 * @param friction The amount of surface friction.
	 * @param linearFactor How effected each axis will be to linear movement.
	 * @param angularFactor How effected each axis will be to angular movement.
	 */
	explicit CollisionObject(const float &mass = 1.0f, const float &friction = 0.2f, const Vector3f &linearFactor = Vector3f(1.0f), const Vector3f &angularFactor = Vector3f(1.0f));

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

	const float &GetMass() const { return m_mass; }

	virtual void SetMass(const float &mass) = 0;

	const Vector3f &GetGravity() const { return m_gravity; }

	virtual void SetGravity(const Vector3f &gravity) = 0;

	const Vector3f &GetLinearFactor() const { return m_linearFactor; }

	virtual void SetLinearFactor(const Vector3f &linearFactor) = 0;

	const Vector3f &GetAngularFactor() const { return m_angularFactor; }

	virtual void SetAngularFactor(const Vector3f &angularFactor) = 0;

	const float &GetFriction() const { return m_friction; }

	void SetFriction(const float &friction);

	const float &GetFrictionRolling() const { return m_frictionRolling; }

	void SetFrictionRolling(const float &frictionRolling);

	const float &GetFrictionSpinning() const { return m_frictionSpinning; }

	void SetFrictionSpinning(const float &frictionSpinning);

	const Vector3f &GetLinearVelocity() const { return m_linearVelocity; }

	virtual void SetLinearVelocity(const Vector3f &linearVelocity) = 0;

	const Vector3f &GetAngularVelocity() const { return m_angularVelocity; }

	virtual void SetAngularVelocity(const Vector3f &angularVelocity) = 0;

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

	float m_mass;
	Vector3f m_gravity;

	float m_friction;
	float m_frictionRolling;
	float m_frictionSpinning;

	Vector3f m_linearFactor;
	Vector3f m_angularFactor;

	Vector3f m_linearVelocity;
	Vector3f m_angularVelocity;

	std::unique_ptr<btCollisionShape> m_shape;
	btCollisionObject *m_body;

	std::vector<std::unique_ptr<Force>> m_forces;

	Delegate<void(CollisionObject *)> m_onCollision;
	Delegate<void(CollisionObject *)> m_onSeparation;
};
}
