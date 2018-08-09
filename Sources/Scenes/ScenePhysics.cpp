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
		m_collisionConfiguration(new btSoftBodyRigidBodyCollisionConfiguration()),
		m_broadphase(new btDbvtBroadphase()),
		m_dispatcher(new btCollisionDispatcher(m_collisionConfiguration)),
		m_solver(new btSequentialImpulseConstraintSolver()),
		m_dynamicsWorld(new btSoftRigidDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration))
	{
		m_dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
		m_dynamicsWorld->getSolverInfo().m_solverMode |= SOLVER_RANDMIZE_ORDER;
		m_dynamicsWorld->getDispatchInfo().m_enableSatConvex = true;
		m_dynamicsWorld->getSolverInfo().m_splitImpulse = true;

		auto softDynamicsWorld = static_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld);
		softDynamicsWorld->getWorldInfo().air_density = 1.0f;
		softDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();
	}

	ScenePhysics::~ScenePhysics()
	{
		for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
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

		delete m_collisionConfiguration;
		delete m_dispatcher;
		delete m_broadphase;
		delete m_solver;
		delete m_dynamicsWorld;
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
		auto softDynamicsWorld = static_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld);
		return softDynamicsWorld->getWorldInfo().air_density;
	}

	void ScenePhysics::SetAirDensity(const float &airDensity)
	{
		auto softDynamicsWorld = static_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld);
		softDynamicsWorld->getWorldInfo().air_density = airDensity;
		softDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();
	}
}
