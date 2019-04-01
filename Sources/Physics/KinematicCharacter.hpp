#pragma once

#include "Maths/Vector3.hpp"
#include "Colliders/Collider.hpp"
#include "CollisionObject.hpp"

class btPairCachingGhostObject;
class btKinematicCharacterController;

namespace acid
{
/**
 * Represents a kinematic character controller.
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
	explicit KinematicCharacter(const float &mass = 1.0f, const float &friction = 0.2f);

	~KinematicCharacter();

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

	const Vector3 &GetUp() const { return m_up; }

	void SetUp(const Vector3 &up);

	const float &GetStepHeight() const { return m_stepHeight; }

	void SetStepHeight(const float &stepHeight);

	const float &GetFallSpeed() const { return m_fallSpeed; }

	void SetFallSpeed(const float &fallSpeed);

	const float &GetJumpSpeed() const { return m_jumpSpeed; }

	void SetJumpSpeed(const float &jumpSpeed);

	const float &GetMaxJumpHeight() const { return m_maxHeight; }

	void SetMaxJumpHeight(const float &maxHeight);

	const bool &IsInterpolate() const { return m_interpolate; }

	void SetInterpolate(const bool &interpolate);

	bool IsOnGround() const;

	void Jump(const Vector3 &direction);

	void SetWalkDirection(const Vector3 &direction);

protected:
	void RecalculateMass() override;

private:
	float m_mass;
	Vector3 m_gravity;
	Vector3 m_up;
	float m_stepHeight;
	float m_fallSpeed;
	float m_jumpSpeed;
	float m_maxHeight;
	bool m_interpolate;

	std::unique_ptr<btPairCachingGhostObject> m_ghostObject;
	std::unique_ptr<btKinematicCharacterController> m_controller;
};
}
