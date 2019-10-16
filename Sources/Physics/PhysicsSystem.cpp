#include "PhysicsSystem.hpp"

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <LinearMath/btDefaultMotionState.h>
#include "Scenes/Entity.inl"
#include "Rigidbody.hpp"
#include "KinematicCharacter.hpp"

namespace acid {
PhysicsSystem::PhysicsSystem() :
	m_collisionConfiguration(std::make_unique<btSoftBodyRigidBodyCollisionConfiguration>()),
	m_broadphase(std::make_unique<btDbvtBroadphase>()),
	m_dispatcher(std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get())),
	m_solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
	m_dynamicsWorld(std::make_unique<btSoftRigidDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get())),
	m_gravity(0.0f, -9.81f, 0.0f),
	m_airDensity(1.2f) {
	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	m_dynamicsWorld->setGravity(Collider::Convert(m_gravity));
	m_dynamicsWorld->getDispatchInfo().m_enableSPU = true;
	m_dynamicsWorld->getSolverInfo().m_minimumSolverBatchSize = 128;
	m_dynamicsWorld->getSolverInfo().m_globalCfm = 0.00001f;

	auto softDynamicsWorld = static_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld.get());
	softDynamicsWorld->getWorldInfo().water_density = 0.0f;
	softDynamicsWorld->getWorldInfo().water_offset = 0.0f;
	softDynamicsWorld->getWorldInfo().water_normal = btVector3(0.0f, 0.0f, 0.0f);
	softDynamicsWorld->getWorldInfo().m_gravity.setValue(0.0f, -9.81f, 0.0f);
	softDynamicsWorld->getWorldInfo().air_density = m_airDensity;
	softDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();

	GetFilter().Set(FilterMatch<Transform>() && (FilterMatch<Rigidbody>() || FilterMatch<KinematicCharacter>()));
}

PhysicsSystem::~PhysicsSystem() {
	for (int32_t i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		auto obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		auto body = btRigidBody::upcast(obj);

		if (body && body->getMotionState()) {
			delete body->getMotionState();
			body->setMotionState(nullptr);
		}

		m_dynamicsWorld->removeCollisionObject(obj);
	}
}

void PhysicsSystem::OnEntityAttach(Entity entity) {
	auto transform = entity.GetComponent<Transform>();

	CollisionObject *collisionObject;
	
	if (auto rigidbody = entity.GetComponent<Rigidbody>()) {
		collisionObject = static_cast<CollisionObject *>(rigidbody);
		
		rigidbody->m_gravity = m_gravity;
		rigidbody->CreateShape();
		assert((!rigidbody->m_shape || rigidbody->m_shape->getShapeType() != INVALID_SHAPE_PROXYTYPE) && "Invalid rigidbody shape!");
		rigidbody->CreateCollisionObject(transform);
		m_dynamicsWorld->addRigidBody(rigidbody->m_rigidBody.get());
		rigidbody->RecalculateMass();
	}

	if (auto character = entity.GetComponent<KinematicCharacter>()) {
		collisionObject = static_cast<CollisionObject *>(character);
		
		character->m_gravity = m_gravity;
		character->CreateShape(true);
		assert((rigidbody->m_shape || rigidbody->m_shape->getShapeType() != INVALID_SHAPE_PROXYTYPE) && "Invalid ghost object shape!");
		character->CreateCollisionObject(transform);
		m_dynamicsWorld->addCollisionObject(character->m_ghostObject.get(), btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
		m_dynamicsWorld->addAction(character->m_controller.get());
		character->RecalculateMass();
	}
}

void PhysicsSystem::OnEntityDetach(Entity entity) {
	if (auto rigidbody = entity.GetComponent<Rigidbody>()) {
		auto body = btRigidBody::upcast(rigidbody->m_body);

		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}

		m_dynamicsWorld->removeRigidBody(rigidbody->m_rigidBody.get());
	}
	
	if (auto character = entity.GetComponent<KinematicCharacter>()) {
		m_dynamicsWorld->removeCollisionObject(character->m_ghostObject.get());
		m_dynamicsWorld->removeAction(character->m_controller.get());
	}
}

void PhysicsSystem::Update(const Time &delta) {
	m_dynamicsWorld->stepSimulation(delta.AsSeconds());
	CheckForCollisionEvents();
	
	ForEach<Transform, Rigidbody, KinematicCharacter>([delta](Entity entity, Transform *transform, Rigidbody *rigidbody, KinematicCharacter *character) {
		CollisionObject *collisionObject = nullptr;

		if (rigidbody) {
			collisionObject = static_cast<CollisionObject *>(rigidbody);
			
			for (auto it = collisionObject->m_forces.begin(); it != collisionObject->m_forces.end();) {
				(*it)->Update(delta);
				rigidbody->m_rigidBody->applyForce(Collider::Convert((*it)->GetForce()), Collider::Convert((*it)->GetPosition()));

				if ((*it)->IsExpired()) {
					it = collisionObject->m_forces.erase(it);
				} else {
					++it;
				}
			}
			
			btTransform motionTransform;
			rigidbody->m_rigidBody->getMotionState()->getWorldTransform(motionTransform);
			*transform = Collider::Convert(motionTransform, transform->GetScale());

			rigidbody->m_shape->setLocalScaling(Collider::Convert(transform->GetScale()));
			//rigidbody->m_rigidBody->getMotionState()->setWorldTransform(Collider::Convert(*transform));
			rigidbody->m_linearVelocity = Collider::Convert(rigidbody->m_rigidBody->getLinearVelocity());
			rigidbody->m_angularVelocity = Collider::Convert(rigidbody->m_rigidBody->getAngularVelocity());
		}

		if (character) {
			collisionObject = static_cast<CollisionObject *>(character);
			
			auto worldTransform = character->m_ghostObject->getWorldTransform();
			*transform = Collider::Convert(worldTransform, transform->GetScale());

			character->m_linearVelocity = Collider::Convert(character->m_controller->getLinearVelocity());
			character->m_angularVelocity = Collider::Convert(character->m_controller->getAngularVelocity());
		}

		if (collisionObject && collisionObject->m_shape.get() != collisionObject->m_body->getCollisionShape()) {
			collisionObject->m_body->setCollisionShape(collisionObject->m_shape.get());
		}
	});
}

Raycast PhysicsSystem::Raytest(const Vector3f &start, const Vector3f &end) const {
	auto startBt = Collider::Convert(start);
	auto endBt = Collider::Convert(end);
	btCollisionWorld::ClosestRayResultCallback result(startBt, endBt);
	m_dynamicsWorld->getCollisionWorld()->rayTest(startBt, endBt, result);

	return Raycast(result.hasHit(), Collider::Convert(result.m_hitPointWorld),
		result.m_collisionObject ? static_cast<CollisionObject *>(result.m_collisionObject->getUserPointer()) : nullptr);
}

void PhysicsSystem::SetGravity(const Vector3f &gravity) {
	m_gravity = gravity;
	m_dynamicsWorld->setGravity(Collider::Convert(m_gravity));
}

void PhysicsSystem::SetAirDensity(float airDensity) {
	m_airDensity = airDensity;
	auto softDynamicsWorld = static_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld.get());
	softDynamicsWorld->getWorldInfo().air_density = m_airDensity;
	softDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();
}

void PhysicsSystem::CheckForCollisionEvents() {
	// Keep a list of the collision pairs found during the current update.
	CollisionPairs pairsThisUpdate;

	// Iterate through all of the manifolds in the dispatcher.
	for (int32_t i = 0; i < m_dispatcher->getNumManifolds(); ++i) {
		// Get the manifold.
		auto manifold = m_dispatcher->getManifoldByIndexInternal(i);

		// Ignore manifolds that have no contact points..
		if (manifold->getNumContacts() == 0) {
			continue;
		}

		// Get the two rigid bodies involved in the collision.
		auto body0 = manifold->getBody0();
		auto body1 = manifold->getBody1();

		// Always create the pair in a predictable order (use the pointer value..).
		const auto swapped = body0 > body1;
		const auto sortedBodyA = swapped ? body1 : body0;
		const auto sortedBodyB = swapped ? body0 : body1;

		// Create the pair.
		auto thisPair = std::make_pair(sortedBodyA, sortedBodyB);

		// Insert the pair into the current list.
		pairsThisUpdate.insert(thisPair);

		// If this pair doesn't exist in the list from the previous update, it is a new pair and we must send a collision event.
		if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end()) {
			// Gets the user pointer (entity).
			auto collisionObjectA = static_cast<CollisionObject *>(sortedBodyA->getUserPointer());
			auto collisionObjectB = static_cast<CollisionObject *>(sortedBodyB->getUserPointer());

			collisionObjectA->OnCollision()(collisionObjectB);
		}
	}

	// Creates another list for pairs that were removed this update.
	CollisionPairs removedPairs;

	// This handy function gets the difference between two sets. It takes the difference between collision pairs from the last update,
	// and this update and pushes them into the removed pairs list.
	std::set_difference(m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(), pairsThisUpdate.begin(), pairsThisUpdate.end(), std::inserter(removedPairs, removedPairs.begin()));

	// Iterate through all of the removed pairs sending separation events for them.
	for (const auto &[removedObject0, removedObject1] : removedPairs) {
		// Gets the user pointer (entity).
		auto collisionObjectA = static_cast<CollisionObject *>(removedObject0->getUserPointer());
		auto collisionObjectB = static_cast<CollisionObject *>(removedObject1->getUserPointer());

		collisionObjectA->OnSeparation()(collisionObjectB);
	}

	// In the next iteration we'll want to compare against the pairs we found in this iteration.
	m_pairsLastUpdate = pairsThisUpdate;
}
}
