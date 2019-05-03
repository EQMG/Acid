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

	bool InFrustum(const Frustum &frustum) override;

	void ClearForces() override;

	void SetMass(const float &mass) override;

	void SetGravity(const Vector3f &gravity) override;

	void SetLinearFactor(const Vector3f &linearFactor) override;

	void SetAngularFactor(const Vector3f &angularFactor) override;

	void SetLinearVelocity(const Vector3f &linearVelocity) override;

	void SetAngularVelocity(const Vector3f &angularVelocity) override;

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, Rigidbody &rigidbody);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const Rigidbody &rigidbody);

protected:
	void RecalculateMass() override;

private:
	std::unique_ptr<btRigidBody> m_rigidBody;
};
}
