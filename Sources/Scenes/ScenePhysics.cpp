#include "ScenePhysics.hpp"

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include "Engine/Engine.hpp"
#include "Objects/GameObject.hpp"
#include "Physics/Collider.hpp"
#include "Physics/CollisionObject.hpp"

namespace acid
{
	ScenePhysics::ScenePhysics() :
		m_collisionConfiguration(std::make_unique<btSoftBodyRigidBodyCollisionConfiguration>()),
		m_broadphase(std::make_unique<btDbvtBroadphase>()),
		m_dispatcher(std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get())),
		m_solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
		m_dynamicsWorld(std::make_unique<btSoftRigidDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get())),
		m_pairsLastUpdate(CollisionPairs())
	{
		m_dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
		m_dynamicsWorld->getSolverInfo().m_solverMode |= SOLVER_RANDMIZE_ORDER;
		m_dynamicsWorld->getDispatchInfo().m_enableSatConvex = true;
		m_dynamicsWorld->getSolverInfo().m_splitImpulse = true;

		auto softDynamicsWorld = dynamic_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld.get());
		softDynamicsWorld->getWorldInfo().air_density = 1.0f;
		softDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();
	}

	ScenePhysics::~ScenePhysics()
	{
		for (int32_t i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject *obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody *body = btRigidBody::upcast(obj);

			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}

			m_dynamicsWorld->removeCollisionObject(obj);
		}
	}

	void ScenePhysics::Update()
	{
		m_dynamicsWorld->stepSimulation(Engine::Get()->GetDelta().AsSeconds());
		CheckForCollisionEvents();
	}

	Raycast ScenePhysics::Raytest(const Vector3 &start, const Vector3 &end)
	{
		auto startBt = Collider::Convert(start);
		auto endBt = Collider::Convert(end);
		btCollisionWorld::ClosestRayResultCallback result(startBt, endBt);
		m_dynamicsWorld->getCollisionWorld()->rayTest(startBt, endBt, result);

		return Raycast(result.hasHit(), Collider::Convert(result.m_hitPointWorld), result.m_collisionObject != nullptr ? static_cast<CollisionObject *>(result.m_collisionObject->getUserPointer()) : nullptr);
	}

	Vector3 ScenePhysics::GetGravity() const
	{
		return Collider::Convert(m_dynamicsWorld->getGravity());
	}

	void ScenePhysics::SetGravity(const Vector3 &gravity)
	{
		m_dynamicsWorld->setGravity(Collider::Convert(gravity));
	}

	float ScenePhysics::GetAirDensity() const
	{
		auto softDynamicsWorld = dynamic_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld.get());
		return softDynamicsWorld->getWorldInfo().air_density;
	}

	void ScenePhysics::SetAirDensity(const float &airDensity)
	{
		auto softDynamicsWorld = dynamic_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld.get());
		softDynamicsWorld->getWorldInfo().air_density = airDensity;
		softDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();
	}

	void ScenePhysics::CheckForCollisionEvents()
	{
		// Keep a list of the collision pairs found during the current update.
		CollisionPairs pairsThisUpdate;

		// Iterate through all of the manifolds in the dispatcher.
		for (int i = 0; i < m_dispatcher->getNumManifolds(); ++i)
		{
			// Get the manifold.
			auto manifold = m_dispatcher->getManifoldByIndexInternal(i);

			// Ignore manifolds that have no contact points..
			if (manifold->getNumContacts() == 0)
			{
				continue;
			}

			// Get the two rigid bodies involved in the collision.
			auto body0 = manifold->getBody0();
			auto body1 = manifold->getBody1();

			// Always create the pair in a predictable order (use the pointer value..).
			const bool swapped = body0 > body1;
			const auto sortedBodyA = swapped ? body1 : body0;
			const auto sortedBodyB = swapped ? body0 : body1;

			// Create the pair.
			auto thisPair = std::make_pair(sortedBodyA, sortedBodyB);

			// Insert the pair into the current list.
			pairsThisUpdate.insert(thisPair);

			// If this pair doesn't exist in the list from the previous update, it is a new pair and we must send a collision event.
			if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end())
			{
				// Gets the user pointer (Game Object).
				auto collisionObjectA = static_cast<CollisionObject *>(sortedBodyA->getUserPointer());
				auto collisionObjectB = static_cast<CollisionObject *>(sortedBodyB->getUserPointer());

				collisionObjectA->GetCollisionEvents().OnEvent(collisionObjectB);
			}
		}

		// create another list for pairs that were removed this update.
		CollisionPairs removedPairs;

		// This handy function gets the difference between two sets. It takes the difference between collision pairs from the last update,
		// and this update and pushes them into the removed pairs list.
		std::set_difference(m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(), pairsThisUpdate.begin(), pairsThisUpdate.end(),
			std::inserter(removedPairs, removedPairs.begin()));

		// Iterate through all of the removed pairs sending separation events for them.
		for (auto it = removedPairs.begin(); it != removedPairs.end(); ++it)
		{
			// Gets the user pointer (Game Object).
			auto collisionObjectA = static_cast<CollisionObject *>(it->first->getUserPointer());
			auto collisionObjectB = static_cast<CollisionObject *>(it->second->getUserPointer());

			collisionObjectA->GetSeparationEvents().OnEvent(collisionObjectB);
		}

		// In the next iteration we'll want to compare against the pairs we found in this iteration.
		m_pairsLastUpdate = pairsThisUpdate;
	}
}
