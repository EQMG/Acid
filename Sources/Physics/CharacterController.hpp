#pragma once

#include "Collider.hpp"
#include "Maths/Vector3.hpp"

class btTransform;

class btPairCachingGhostObject;

class btKinematicCharacterController;

namespace acid
{
	class ACID_EXPORT CharacterController :
		public IComponent
	{
	private:
		btCollisionShape* m_shape;
		std::unique_ptr<btPairCachingGhostObject> m_ghostObject;
		std::unique_ptr<btKinematicCharacterController> m_controller;
	public:
		explicit CharacterController();

		~CharacterController();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void SetGravity(const Vector3 &gravity);

		void SetUp(const Vector3 &up);

		void SetStepHeight(const float &stepHeight);

		void SetFallSpeed(const float &fallSpeed);

		void SetJumpSpeed(const float &jumpSpeed);

		void SetMaxJumpHeight(const float &maxHeight);

		void SetInterpolate(const bool &interpolate);

		bool IsOnGround() const;

		void Jump(const Vector3 &direction);

		void SetWalkDirection(const Vector3 &direction);
	private:
		static std::unique_ptr<btPairCachingGhostObject> CreateGhostObject(float mass, const btTransform &startTransform, btCollisionShape* shape);
	};
}
