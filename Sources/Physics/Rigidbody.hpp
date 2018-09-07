#pragma once

#include "Maths/Vector3.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/IComponent.hpp"
#include "Force.hpp"

class btTransform;

class btCollisionShape;

class btRigidBody;

namespace acid
{
	class ACID_EXPORT Rigidbody :
		public IComponent
	{
	private:
		float m_mass;
		float m_friction;
		Vector3 m_linearFactor;
		Vector3 m_angularFactor;

		btCollisionShape* m_shape;
		std::unique_ptr<btRigidBody> m_body;

		std::vector<std::unique_ptr<Force>> m_forces;

		Vector3 m_linearVelocity;
		Vector3 m_angularVelocity;
	public:
		Rigidbody(const float &mass = 1.0f, const float &friction = 0.2f, const Vector3 &linearFactor = Vector3::ONE, const Vector3 &angularFactor = Vector3::ONE);

		~Rigidbody();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		void SetGravity(const Vector3 &gravity);

		Force *AddForce(Force *force);

		template<typename T, typename... Args>
		Force *AddForce(Args &&... args) { return AddForce(new T(std::forward<Args>(args)...)); }

		void ClearForces();

		float GetMass() const { return m_mass; }

		void SetMass(const float &mass);

		float GetFriction() const { return m_friction; }

		void SetFriction(const float &friction);

		Vector3 GetLinearFactor() const { return m_linearFactor; }

		void SetLinearFactor(const Vector3 &linearFactor);

		Vector3 GetAngularFactor() const { return m_angularFactor; }

		void SetAngularFactor(const Vector3 &angularFactor);

		const Vector3 GetLinearVelocity() const { return m_linearVelocity; }

		void SetLinearVelocity(const Vector3 &linearVelocity);

		const Vector3 GetAngularVelocity() const { return m_angularVelocity; }

		void SetAngularVelocity(const Vector3 &angularVelocity);
	private:
		static std::unique_ptr<btRigidBody> CreateRigidBody(float mass, const btTransform &startTransform, btCollisionShape* shape);
	};
}
