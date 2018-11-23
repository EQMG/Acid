#pragma once

#include "Events/Observer.hpp"
#include "Maths/Vector3.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/IComponent.hpp"
#include "CollisionObject.hpp"

class btRigidBody;

namespace acid
{
	/// <summary>
	/// Represents a object in a scene effected by physics.
	/// </summary>
	class ACID_EXPORT Rigidbody :
		public CollisionObject
	{
	private:
		float m_mass;

		Vector3 m_gravity;

		Vector3 m_linearFactor;
		Vector3 m_angularFactor;

		Vector3 m_linearVelocity;
		Vector3 m_angularVelocity;

		btRigidBody *m_rigidBody;
	public:
		/// <summary>
		/// Creates a new rigidbody.
		/// </summary>
		/// <param name="mass"> The mass of the object. </param>
		/// <param name="friction"> The amount of surface friction. </param>
		/// <param name="linearFactor"> How effected each axis (XYZ) will be to linear movement. </param>
		/// <param name="angularFactor"> How effected each axis (XYZ) will be to angular movement. </param>
		explicit Rigidbody(const float &mass = 1.0f, const float &friction = 0.2f, const Vector3 &linearFactor = Vector3::ONE, const Vector3 &angularFactor = Vector3::ONE);

		~Rigidbody();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		bool InFrustum(const Frustum &frustum) override;

		void ClearForces() override;

		float GetMass() const { return m_mass; }

		void SetMass(const float &mass);

		Vector3 GetGravity() const { return m_gravity; }

		void SetGravity(const Vector3 &gravity);

		Vector3 GetLinearFactor() const { return m_linearFactor; }

		void SetLinearFactor(const Vector3 &linearFactor);

		Vector3 GetAngularFactor() const { return m_angularFactor; }

		void SetAngularFactor(const Vector3 &angularFactor);

		const Vector3 GetLinearVelocity() const { return m_linearVelocity; }

		void SetLinearVelocity(const Vector3 &linearVelocity);

		const Vector3 GetAngularVelocity() const { return m_angularVelocity; }

		void SetAngularVelocity(const Vector3 &angularVelocity);
	protected:
		void RecalculateMass() override;
	private:
		static std::unique_ptr<btRigidBody> CreateRigidBody(float mass, const btTransform &startTransform, btCollisionShape* shape);
	};
}
