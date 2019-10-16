#pragma once

#include "Maths/Vector3.hpp"
#include "Scenes/System.hpp"
#include "CollisionObject.hpp"

class btCollisionObject;
class btCollisionConfiguration;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDiscreteDynamicsWorld;

namespace acid {
using CollisionPair = std::pair<const btCollisionObject *, const btCollisionObject *>;
using CollisionPairs = std::set<CollisionPair>;

class ACID_EXPORT Raycast {
public:
	Raycast(bool hasHit, const Vector3f &pointWorld, CollisionObject *collisionObject) :
		m_hasHit(hasHit),
		m_pointWorld(pointWorld),
		m_collisionObject(collisionObject) {
	}

	bool HasHit() const { return m_hasHit; }
	const Vector3f &GetPointWorld() const { return m_pointWorld; }
	CollisionObject *GetCollisionObject() const { return m_collisionObject; }

private:
	bool m_hasHit;
	Vector3f m_pointWorld;
	CollisionObject *m_collisionObject;
};

class ACID_EXPORT PhysicsSystem : public System {
public:
	PhysicsSystem();

	~PhysicsSystem();

	void OnEntityAttach(Entity entity) ;
	void OnEntityDetach(Entity entity) override;
	void Update(const Time &delta) override;
	
	Raycast Raytest(const Vector3f &start, const Vector3f &end) const;

	const Vector3f &GetGravity() const { return m_gravity; }
	void SetGravity(const Vector3f &gravity);

	float GetAirDensity() const { return m_airDensity; }
	void SetAirDensity(float airDensity);

	btBroadphaseInterface *GetBroadphase() { return m_broadphase.get(); }

	btDiscreteDynamicsWorld *GetDynamicsWorld() { return m_dynamicsWorld.get(); }

private:
	void CheckForCollisionEvents();

	std::unique_ptr<btCollisionConfiguration> m_collisionConfiguration;
	std::unique_ptr<btBroadphaseInterface> m_broadphase;
	std::unique_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btConstraintSolver> m_solver;
	std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
	CollisionPairs m_pairsLastUpdate;

	Vector3f m_gravity;
	float m_airDensity;
};
}
