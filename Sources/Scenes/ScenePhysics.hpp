#pragma once

#include <memory>
#include <optional>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include "Maths/Vector3.hpp"

class btCollisionConfiguration;

class btBroadphaseInterface;

class btCollisionDispatcher;

class btSequentialImpulseConstraintSolver;

class btDiscreteDynamicsWorld;

namespace acid
{
	class GameObject;

	class ACID_EXPORT Raycast
	{
	private:
		bool m_hasHit;
		Vector3 m_pointWorld;
		GameObject *m_gameObject;
	public:
		Raycast(bool m_hasHit, const Vector3 &m_pointWorld, GameObject *gameObject) :
			m_hasHit(m_hasHit),
			m_pointWorld(m_pointWorld),
			m_gameObject(gameObject)
		{
		}

		bool HasHit() const
		{
			return m_hasHit;
		}

		const Vector3 &GetPointWorld() const
		{
			return m_pointWorld;
		}

		GameObject *GetGameObject() const
		{
			return m_gameObject;
		}
	};

	class ACID_EXPORT ScenePhysics
	{
	private:
		std::unique_ptr<btCollisionConfiguration> m_collisionConfiguration;
		std::unique_ptr<btBroadphaseInterface> m_broadphase;
		std::unique_ptr<btCollisionDispatcher> m_dispatcher;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
		std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
	public:
		ScenePhysics();

		~ScenePhysics();

		void Update();

		Raycast Raytest(const Vector3 &start, const Vector3 &end);

		Vector3 GetGravity() const;

		void SetGravity(const Vector3 &gravity);

		float GetAirDensity() const;

		void SetAirDensity(const float &airDensity);

		btBroadphaseInterface *GetBroadphase() { return m_broadphase.get(); }

		btDiscreteDynamicsWorld *GetDynamicsWorld() { return m_dynamicsWorld.get(); }
	};
}
