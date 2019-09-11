#pragma once

#include "Maths/Vector3.hpp"
#include "Colliders/Collider.hpp"
#include "CollisionObject.hpp"

class btPairCachingGhostObject;
class btKinematicCharacterController;

namespace acid
{
/**
 * @brief Represents a kinematic character controller.
 */
class ACID_EXPORT KinematicCharacter :
	public CollisionObject
{
public:
	/**
	 * Creates a new kinematic character controller.
	 * @param mass The mass of the object.
	 * @param friction The amount of surface friction.
	 */
	explicit KinematicCharacter(float mass = 1.0f, float friction = 0.2f);

	~KinematicCharacter();

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

	const Vector3f &GetUp() const { return m_up; }

	void SetUp(const Vector3f &up);

	float GetStepHeight() const { return m_stepHeight; }

	void SetStepHeight(float stepHeight);

	float GetFallSpeed() const { return m_fallSpeed; }

	void SetFallSpeed(float fallSpeed);

	float GetJumpSpeed() const { return m_jumpSpeed; }

	void SetJumpSpeed(float jumpSpeed);

	float GetMaxJumpHeight() const { return m_maxHeight; }

	void SetMaxJumpHeight(float maxHeight);

	bool IsInterpolate() const { return m_interpolate; }

	void SetInterpolate(bool interpolate);

	bool IsOnGround() const;

	void Jump(const Vector3f &direction);

	void SetWalkDirection(const Vector3f &direction);

	friend const Node &operator>>(const Node &node, KinematicCharacter &character);

	friend Node &operator<<(Node &node, const KinematicCharacter &character);

protected:
	void RecalculateMass() override;

private:
	Vector3f m_up;
	float m_stepHeight;
	float m_fallSpeed;
	float m_jumpSpeed;
	float m_maxHeight;
	bool m_interpolate;

	std::unique_ptr<btPairCachingGhostObject> m_ghostObject;
	std::unique_ptr<btKinematicCharacterController> m_controller;
};
}
