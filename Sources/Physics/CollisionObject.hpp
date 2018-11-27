#pragma once

#include <memory>
#include "Events/Observer.hpp"
#include "Maths/Vector3.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/IComponent.hpp"
#include "Force.hpp"

class btTransform;

class btCollisionShape;

class btCollisionObject;

namespace acid
{
	/// <summary>
	/// Represents a object in a scene effected by physics.
	/// </summary>
	class ACID_EXPORT CollisionObject :
		public IComponent
	{
	protected:
		float m_friction;
		float m_frictionRolling;
		float m_frictionSpinning;

		std::unique_ptr<btCollisionShape> m_shape;
		std::unique_ptr<btCollisionObject> m_body;

		std::vector<std::unique_ptr<Force>> m_forces;

		Observer<CollisionObject *> m_collisionEvents;
		Observer<CollisionObject *> m_separationEvents;
	public:
		/// <summary>
		/// Creates a new collision object.
		/// </summary>
		/// <param name="friction"> The amount of surface friction. </param>
		/// <param name="localTransform"> The parent offset of the body. </param>
		explicit CollisionObject(const float &friction = 0.2f);

		~CollisionObject();

		/// <summary>
		/// Gets if the shape is partially in the view frustum.
		/// </summary>
		/// <param name="frustum"> The view frustum. </param>
		/// <returns> If the shape is partially in the view frustum. </returns>
		virtual bool InFrustum(const Frustum &frustum) = 0;

		Force *AddForce(Force *force);

		template<typename T, typename... Args>
		Force *AddForce(Args &&... args) { return AddForce(new T(std::forward<Args>(args)...)); }

		virtual void ClearForces() = 0;

		bool IsShapeCreated() const { return IsStarted() && m_shape != nullptr; }

		void SetChildTransform(Collider *child, const Transform &transform);

		void AddChild(Collider *child);

		void RemoveChild(Collider *child);

		void SetIgnoreCollisionCheck(CollisionObject *other, const bool &ignore);

		float GetFriction() const { return m_friction; }

		void SetFriction(const float &friction);

		float GetFrictionRolling() const { return m_frictionRolling; }

		void SetFrictionRolling(const float &frictionRolling);

		float GetFrictionSpinning() const { return m_frictionSpinning; }

		void SetFrictionSpinning(const float &frictionSpinning);

		Observer<CollisionObject *> &GetCollisionEvents() { return m_collisionEvents; }

		Observer<CollisionObject *> &GetSeparationEvents() { return m_separationEvents; }
	protected:
		void CreateShape();

		virtual void RecalculateMass() = 0;
	};
}
