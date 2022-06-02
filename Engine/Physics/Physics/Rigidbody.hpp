#pragma once

#include "Maths/Vector3.hpp"
#include "Scenes/Component.hpp"
#include "CollisionObject.hpp"

struct btDefaultMotionState;
class btRigidBody;

namespace acid {
/**
 * @brief Represents a object in a scene effected by physics.
 */
class ACID_PHYSICS_EXPORT Rigidbody : public Component::Registrar<Rigidbody>, public CollisionObject {
	inline static const bool Registered = Register("rigidbody");
public:
	/**
	 * Creates a new rigidbody.
	 * @param collider The collider shape to represent this rigidbody.
	 * @param mass The mass of the object.
	 * @param friction The amount of surface friction.
	 * @param linearFactor How effected each axis will be to linear movement.
	 * @param angularFactor How effected each axis will be to angular movement.
	 */
	explicit Rigidbody(std::unique_ptr<Collider> &&collider = nullptr, float mass = 1.0f, float friction = 0.2f,
		const Vector3f &linearFactor = Vector3f(1.0f), const Vector3f &angularFactor = Vector3f(1.0f));
	~Rigidbody();

	void Start() override;
	void Update() override;

	bool InFrustum(const Frustum &frustum) override;
	void ClearForces() override;
	void SetMass(float mass) override;
	void SetGravity(const Vector3f &gravity) override;
	void SetLinearFactor(const Vector3f &linearFactor) override;
	void SetAngularFactor(const Vector3f &angularFactor) override;
	void SetLinearVelocity(const Vector3f &linearVelocity) override;
	void SetAngularVelocity(const Vector3f &angularVelocity) override;

	friend const Node &operator>>(const Node &node, Rigidbody &rigidbody);
	friend Node &operator<<(Node &node, const Rigidbody &rigidbody);

protected:
	void RecalculateMass() override;

private:
	std::unique_ptr<btRigidBody> rigidBody;
};
}
