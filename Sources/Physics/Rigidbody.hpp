#pragma once

#include "Maths/Vector3.hpp"
#include "Scenes/Entity.hpp"
#include "CollisionObject.hpp"

struct btDefaultMotionState;
class btRigidBody;

namespace acid
{
/**
 * @brief Represents a object in a scene effected by physics.
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
	explicit Rigidbody(const float &mass = 1.0f, const float &friction = 0.2f, const Vector3f &linearFactor = Vector3f::One, const Vector3f &angularFactor = Vector3f::One);

	~Rigidbody();

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	bool InFrustum(const Frustum &frustum) override;

	void ClearForces() override;

	const float &GetMass() const { return m_mass; }

	void SetMass(const float &mass);

	const Vector3f &GetGravity() const { return m_gravity; }

	void SetGravity(const Vector3f &gravity);

	const Vector3f &GetLinearFactor() const { return m_linearFactor; }

	void SetLinearFactor(const Vector3f &linearFactor);

	const Vector3f &GetAngularFactor() const { return m_angularFactor; }

	void SetAngularFactor(const Vector3f &angularFactor);

	const Vector3f &GetLinearVelocity() const { return m_linearVelocity; }

	void SetLinearVelocity(const Vector3f &linearVelocity);

	const Vector3f &GetAngularVelocity() const { return m_angularVelocity; }

	void SetAngularVelocity(const Vector3f &angularVelocity);

protected:
	void RecalculateMass() override;

private:
	float m_mass;
	Vector3f m_gravity;

	Vector3f m_linearFactor;
	Vector3f m_angularFactor;

	Vector3f m_linearVelocity;
	Vector3f m_angularVelocity;

	std::unique_ptr<btRigidBody> m_rigidBody;
};
}
