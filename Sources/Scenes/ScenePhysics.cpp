#include "ScenePhysics.hpp"

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include "Engine/Engine.hpp"
#include "Physics/Collider.hpp"

namespace acid
{
	ScenePhysics::ScenePhysics() :
		m_collisionConfiguration(std::make_unique<btSoftBodyRigidBodyCollisionConfiguration>()),
		m_broadphase(std::make_unique<btDbvtBroadphase>()),
		m_dispatcher(std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get())),
		m_solver(std::make_unique<btSequentialImpulseConstraintSolver>()),
		m_dynamicsWorld(std::make_unique<btSoftRigidDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get()))
	{
		m_dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
		m_dynamicsWorld->getSolverInfo().m_solverMode |= SOLVER_RANDMIZE_ORDER;
		m_dynamicsWorld->getDispatchInfo().m_enableSatConvex = true;
		m_dynamicsWorld->getSolverInfo().m_splitImpulse = true;

		auto softDynamicsWorld = static_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld.get());
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
			delete obj;
		}
	}

	void ScenePhysics::Update()
	{
		m_dynamicsWorld->stepSimulation(Engine::Get()->GetDelta());
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
		auto softDynamicsWorld = static_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld.get());
		return softDynamicsWorld->getWorldInfo().air_density;
	}

	void ScenePhysics::SetAirDensity(const float &airDensity)
	{
		auto softDynamicsWorld = static_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld.get());
		softDynamicsWorld->getWorldInfo().air_density = airDensity;
		softDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();
	}
}
