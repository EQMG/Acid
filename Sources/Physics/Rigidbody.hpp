#pragma once

#include "Maths/Vector3.hpp"
#include "Scenes/Entity.hpp"
#include "CollisionObject.hpp"

struct btDefaultMotionState;
class btRigidBody;

namespace acid
{
/**
 * Represents a object in a scene effected by physics.
 */
class ACID_EXPORT Rigidbody :
	public CollisionObject
{
public:
	/**
	 * Creates a new rigidbody.
	 * @param mass The mass of the object.
	 * @param friction The amount of surface friction.
	 * @param linearFactor How effected each axis will be to linear movement.
	 * @param angularFactor How effected each axis will be to angular movement.
	 */
	explicit Rigidbody(const float &mass = 1.0f, const float &friction = 0.2f, const Vector3 &linearFactor = Vector3::One, const Vector3 &angularFactor = Vector3::One);

	~Rigidbody();

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	bool InFrustum(const Frustum &frustum) override;

	void ClearForces() override;

	const float &GetMass() const { return m_mass; }

	void SetMass(const float &mass);

	const Vector3 &GetGravity() const { return m_gravity; }

	void SetGravity(const Vector3 &gravity);

	const Vector3 &GetLinearFactor() const { return m_linearFactor; }

	void SetLinearFactor(const Vector3 &linearFactor);

	const Vector3 &GetAngularFactor() const { return m_angularFactor; }

	void SetAngularFactor(const Vector3 &angularFactor);

	const Vector3 &GetLinearVelocity() const { return m_linearVelocity; }

	void SetLinearVelocity(const Vector3 &linearVelocity);

	const Vector3 &GetAngularVelocity() const { return m_angularVelocity; }

	void SetAngularVelocity(const Vector3 &angularVelocity);

protected:
	void RecalculateMass() override;

private:
	float m_mass;
	Vector3 m_gravity;

	Vector3 m_linearFactor;
	Vector3 m_angularFactor;

	Vector3 m_linearVelocity;
	Vector3 m_angularVelocity;

	std::unique_ptr<btRigidBody> m_rigidBody;
};
}
