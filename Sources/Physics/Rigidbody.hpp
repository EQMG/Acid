#pragma once

#include <BulletDynamics/Dynamics/btRigidBody.h>
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
		Vector3 m_linearFactor;
		Vector3 m_angularFactor;

		btTransform m_worldTransform;
		btCollisionShape *m_shape;
		btRigidBody *m_body;
	public:
		Rigidbody(const float &mass = 1.0f, const float &friction = 0.2f, const Vector3 &linearFactor = Vector3::ONE,
			const Vector3 &angularFactor = Vector3::ONE);

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

		Vector3 GetLinearFactor() const { return m_linearFactor; }

		void SetLinearFactor(const Vector3 &linearFactor);

		Vector3 GetAngularFactor() const { return m_angularFactor; }

		void SetAngularFactor(const Vector3 &angularFactor);
	private:
		static btRigidBody *CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);
	};
}
