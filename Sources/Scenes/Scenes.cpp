#include "Scenes.hpp"

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

namespace acid
{
	Scenes::Scenes() :
		IModule(),
		m_scene(nullptr),
		m_componentRegister(ComponentRegister())
	{
	//	m_collisionConfiguration = new btDefaultCollisionConfiguration();
		m_collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

		m_broadphase = new btDbvtBroadphase();

		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

		m_solver = new btSequentialImpulseConstraintSolver();

	//	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
		m_dynamicsWorld = new btSoftRigidDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

		m_dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
		m_dynamicsWorld->getSolverInfo().m_solverMode |= SOLVER_RANDMIZE_ORDER;
		m_dynamicsWorld->getDispatchInfo().m_enableSatConvex = true;
		m_dynamicsWorld->getSolverInfo().m_splitImpulse = true;

		auto softRigidDynamicsWorld = static_cast<btSoftRigidDynamicsWorld *>(m_dynamicsWorld);
		softRigidDynamicsWorld->getWorldInfo().air_density = 1.0f;
		softRigidDynamicsWorld->getWorldInfo().water_density = 0.0f;
		softRigidDynamicsWorld->getWorldInfo().water_offset = 0.0f;
		softRigidDynamicsWorld->getWorldInfo().water_normal = btVector3(0.0f, 0.0f, 0.0f);
		softRigidDynamicsWorld->getWorldInfo().m_gravity.setValue(0.0f, -9.81f, 0.0f);
		softRigidDynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();
	}

	Scenes::~Scenes()
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

		delete m_scene;
	}

	void Scenes::Update()
	{
		if (m_scene == nullptr)
		{
			return;
		}

		if (!m_scene->IsStarted())
		{
			m_scene->Start();
			m_scene->SetStarted(true);
		}

		m_dynamicsWorld->stepSimulation(Engine::Get()->GetDelta(), 2, 1.0f / 60.0f);
		m_scene->Update();

		if (m_scene->GetStructure() == nullptr)
		{
			return;
		}

		auto gameObjects = m_scene->GetStructure()->GetAll();

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			if ((*it) == nullptr)
			{
				continue;
			}

			(*it)->Update();
		}

		if (m_scene->GetCamera() == nullptr)
		{
			return;
		}

		m_scene->GetCamera()->Update();
	}

	void Scenes::SetScene(IScene *scene)
	{
		delete m_scene;
		m_scene = scene;
	}
}
