#pragma once

#include <BulletDynamics/Dynamics/btRigidBody.h>
#include "Maths/Constraint3.hpp"
#include "Maths/Vector3.hpp"
#include "Objects/IComponent.hpp"
#include "Objects/GameObject.hpp"

namespace fl
{
	class FL_EXPORT Rigidbody :
		public IComponent
	{
	private:
		float m_mass;
		float m_friction;
		Constraint3 m_freezePosition;
		Constraint3 m_freezeRotation;

		btTransform m_worldTransform;
		btCollisionShape *m_shape;
		btRigidBody *m_body;
	public:
		Rigidbody(const float &mass = 1.0f, const float &friction = 0.2f, const Constraint3 &freezePosition = Constraint3::ZERO,
			const Constraint3 &freezeRotation = Constraint3::ZERO);

		~Rigidbody();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Rigidbody"; };

		void SetAngularVelocity(const Vector3 &velocity);

		void SetLinearVelocity(const Vector3 &velocity);

		void AddForce(const Vector3 &force, const Vector3 &position);

		void ClearForces();

		float GetMass() const { return m_mass; }

		void SetMass(const float &mass);

		float GetFriction() const { return m_friction; }

		void SetFriction(const float &friction);

		Constraint3 GetFreezePosition() const { return m_freezePosition; }

		void SetFreezePosition(const Constraint3 &freezePosition);

		Constraint3 GetFreezeRotation() const { return m_freezeRotation; }

		void SetFreezeRotation(const Constraint3 &freezeRotation);
	private:
		static btRigidBody *CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);
	};
}
