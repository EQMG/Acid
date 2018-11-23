#pragma once

#include "Collider.hpp"
#include "Maths/Vector3.hpp"
#include "CollisionObject.hpp"

class btPairCachingGhostObject;

class btKinematicCharacterController;

namespace acid
{
	/// <summary>
	/// Represents a kinematic character controller.
	/// </summary>
	class ACID_EXPORT KinematicCharacter :
		public CollisionObject
	{
	private:
		std::unique_ptr<btKinematicCharacterController> m_controller;

		Vector3 m_gravity;
		Vector3 m_up;
		float m_stepHeight;
		float m_fallSpeed;
		float m_jumpSpeed;
		float m_maxHeight;
		bool m_interpolate;

		btPairCachingGhostObject *m_ghostObject;
	public:
		/// <summary>
		/// Creates a new kinematic character controller.
		/// </summary>
		/// <param name="friction"> The amount of surface friction. </param>
		explicit KinematicCharacter(const float &friction = 0.2f);

		~KinematicCharacter();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		bool InFrustum(const Frustum &frustum) override;

		void ClearForces() override;

		Vector3 GetGravity() const { return m_gravity; }

		void SetGravity(const Vector3 &gravity);

		Vector3 GetUp() const { return m_up; }

		void SetUp(const Vector3 &up);

		float GetStepHeight() const { return m_stepHeight; }

		void SetStepHeight(const float &stepHeight);

		float GetFallSpeed() const { return m_fallSpeed; }

		void SetFallSpeed(const float &fallSpeed);

		float GetJumpSpeed() const { return m_jumpSpeed; }

		void SetJumpSpeed(const float &jumpSpeed);

		float GetMaxJumpHeight() const { return m_maxHeight; }

		void SetMaxJumpHeight(const float &maxHeight);

		bool IsInterpolate() const { return m_interpolate; }

		void SetInterpolate(const bool &interpolate);

		bool IsOnGround() const;

		void Jump(const Vector3 &direction);

		void SetWalkDirection(const Vector3 &direction);
	protected:
		void RecalculateMass() override;
	private:
		static std::unique_ptr<btPairCachingGhostObject> CreateGhostObject(float mass, const btTransform &startTransform, btCollisionShape* shape);
	};
}
