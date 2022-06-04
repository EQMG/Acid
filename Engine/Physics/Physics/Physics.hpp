#pragma once

#include <set>
#include <memory>

#include "Maths/Vector3.hpp"
#include "Scenes/System.hpp"
#include "PhysicsExport.hpp"

class btCollisionObject;
class btCollisionConfiguration;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDiscreteDynamicsWorld;

namespace acid {
class Entity;
class CollisionObject;

using CollisionPair = std::pair<const btCollisionObject *, const btCollisionObject *>;
using CollisionPairs = std::set<CollisionPair>;

class ACID_PHYSICS_EXPORT Raycast {
public:
	Raycast(bool hasHit, const Vector3f &pointWorld, CollisionObject *collisionObject) :
		hasHit(hasHit),
		pointWorld(pointWorld),
		collisionObject(collisionObject) {
	}

	bool HasHit() const { return hasHit; }
	const Vector3f &GetPointWorld() const { return pointWorld; }
	CollisionObject *GetCollisionObject() const { return collisionObject; }

private:
	bool hasHit;
	Vector3f pointWorld;
	CollisionObject *collisionObject;
};

class ACID_PHYSICS_EXPORT Physics : public System {
public:
	Physics();
	~Physics();

	void Update() override;

	Raycast Raytest(const Vector3f &start, const Vector3f &end) const;

	const Vector3f &GetGravity() const { return gravity; }
	void SetGravity(const Vector3f &gravity);

	float GetAirDensity() const { return airDensity; }
	void SetAirDensity(float airDensity);

	btBroadphaseInterface *GetBroadphase() { return broadphase.get(); }

	btDiscreteDynamicsWorld *GetDynamicsWorld() { return dynamicsWorld.get(); }

private:
	void CheckForCollisionEvents();

	std::unique_ptr<btCollisionConfiguration> collisionConfiguration;
	std::unique_ptr<btBroadphaseInterface> broadphase;
	std::unique_ptr<btCollisionDispatcher> dispatcher;
	std::unique_ptr<btConstraintSolver> solver;
	std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
	CollisionPairs pairsLastUpdate;

	Vector3f gravity;
	float airDensity;
};
}
