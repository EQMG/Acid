#pragma once

#include "CollisionObject.hpp"
#include "Maths/Vector3.hpp"
#include "Scenes/Entity.hpp"

struct btDefaultMotionState;
class btRigidBody;

namespace acid
{
/// <summary>
/// Represents
/// a
/// object
/// in a
/// scene
/// effected
/// by
/// physics.
/// </summary>
class ACID_EXPORT Rigidbody : public CollisionObject
{
  public:
	/// <summary>
	/// Creates
	/// a
	/// new
	/// rigidbody.
	/// </summary>
	/// <param
	/// name="mass">
	/// The
	/// mass
	/// of
	/// the
	/// object.
	/// </param>
	/// <param
	/// name="friction">
	/// The
	/// amount
	/// of
	/// surface
	/// friction.
	/// </param>
	/// <param
	/// name="linearFactor">
	/// How
	/// effected
	/// each
	/// axis
	/// (XYZ)
	/// will
	/// be
	/// to
	/// linear
	/// movement.
	/// </param>
	/// <param
	/// name="angularFactor">
	/// How
	/// effected
	/// each
	/// axis
	/// (XYZ)
	/// will
	/// be
	/// to
	/// angular
	/// movement.
	/// </param>
	explicit Rigidbody(const float& mass = 1.0f, const float& friction = 0.2f, const Vector3& linearFactor = Vector3::One, const Vector3& angularFactor = Vector3::One);

	~Rigidbody();

	void Start() override;

	void Update() override;

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

	bool InFrustum(const Frustum& frustum) override;

	void ClearForces() override;

	const float& GetMass() const
	{
		return m_mass;
	}

	void SetMass(const float& mass);

	const Vector3& GetGravity() const
	{
		return m_gravity;
	}

	void SetGravity(const Vector3& gravity);

	const Vector3& GetLinearFactor() const
	{
		return m_linearFactor;
	}

	void SetLinearFactor(const Vector3& linearFactor);

	const Vector3& GetAngularFactor() const
	{
		return m_angularFactor;
	}

	void SetAngularFactor(const Vector3& angularFactor);

	const Vector3& GetLinearVelocity() const
	{
		return m_linearVelocity;
	}

	void SetLinearVelocity(const Vector3& linearVelocity);

	const Vector3& GetAngularVelocity() const
	{
		return m_angularVelocity;
	}

	void SetAngularVelocity(const Vector3& angularVelocity);

  protected:
	void RecalculateMass() override;

  private:
	static btRigidBody* CreateRigidBody(float mass, btDefaultMotionState* motionState, btCollisionShape* shape);

	float m_mass;
	Vector3 m_gravity;

	Vector3 m_linearFactor;
	Vector3 m_angularFactor;

	Vector3 m_linearVelocity;
	Vector3 m_angularVelocity;

	std::unique_ptr<btDefaultMotionState> m_motionState;
	btRigidBody* m_rigidBody;
};
}
