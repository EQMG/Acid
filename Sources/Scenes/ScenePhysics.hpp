#pragma once

#include <set>
#include <iterator>
#include <algorithm>
#include <memory>
#include <optional>
#include "Maths/Vector3.hpp"

class btCollisionObject;

class btCollisionConfiguration;

class btBroadphaseInterface;

class btCollisionDispatcher;

class btSequentialImpulseConstraintSolver;

class btDiscreteDynamicsWorld;

namespace acid
{
	class GameObject;
	class CollisionObject;

	typedef std::pair<const btCollisionObject*, const btCollisionObject*> CollisionPair;
	typedef std::set<CollisionPair> CollisionPairs;

	class ACID_EXPORT Raycast
	{
	private:
		bool m_hasHit;
		Vector3 m_pointWorld;
		CollisionObject *m_collisionObject;
	public:
		Raycast(bool m_hasHit, const Vector3 &m_pointWorld, CollisionObject *collisionObject) :
			m_hasHit(m_hasHit),
			m_pointWorld(m_pointWorld),
			m_collisionObject(collisionObject)
		{
		}

		bool HasHit() const { return m_hasHit; }

		const Vector3 &GetPointWorld() const { return m_pointWorld; }

		CollisionObject *GetCollisionObject() const { return m_collisionObject; }
	};

	class ACID_EXPORT ScenePhysics
	{
	private:
		std::unique_ptr<btCollisionConfiguration> m_collisionConfiguration;
		std::unique_ptr<btBroadphaseInterface> m_broadphase;
		std::unique_ptr<btCollisionDispatcher> m_dispatcher;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
		std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

		CollisionPairs m_pairsLastUpdate;
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
	private:
		void CheckForCollisionEvents();
	};
}
