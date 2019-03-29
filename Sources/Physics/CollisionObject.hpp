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

/// <summary>
/// Represents a object in a scene effected by physics.
/// </summary>
class ACID_EXPORT CollisionObject :
	public Component
{
public:
	/// <summary>
	/// Creates a new collision object.
	/// </summary>
	/// <param name="friction"> The amount of surface friction. </param>
	/// <param name="localTransform"> The parent offset of the body. </param>
	explicit CollisionObject(const float &friction = 0.2f);

	virtual ~CollisionObject();

	/// <summary>
	/// Gets if the shape is partially in the view frustum.
	/// </summary>
	/// <param name="frustum"> The view frustum. </param>
	/// <returns> If the shape is partially in the view frustum. </returns>
	virtual bool InFrustum(const Frustum &frustum) = 0;

	Force *AddForce(Force *force);

	template<typename T, typename... Args> Force *AddForce(Args &&... args) { return AddForce(new T(std::forward<Args>(args)...)); }

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

	Delegate<void(CollisionObject *)> &GetOnCollision() { return m_onCollision; }

	Delegate<void(CollisionObject *)> &GetOnSeparation() { return m_onSeparation; }

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
