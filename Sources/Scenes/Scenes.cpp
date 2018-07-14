#include "Scenes.hpp"

#include <cassert>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <LinearMath/btDefaultMotionState.h>

namespace fl
{
	Scenes::Scenes() :
		IModule(),
		m_scene(nullptr),
		m_componentRegister(ComponentRegister())
	{
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		//m_collisionConfiguration->setConvexConvexMultipointIterations();

		m_broadphase = new btDbvtBroadphase();

		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

		m_solver = new btSequentialImpulseConstraintSolver();

		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

		m_dynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));
		m_dynamicsWorld->getSolverInfo().m_erp2 = 0.0f;
		m_dynamicsWorld->getSolverInfo().m_globalCfm = 0.0f;
		m_dynamicsWorld->getSolverInfo().m_numIterations = 3;
		m_dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_SIMD; // | SOLVER_RANDMIZE_ORDER;
		m_dynamicsWorld->getSolverInfo().m_splitImpulse = false;

		m_collisionShapes = btAlignedObjectArray<btCollisionShape *>();
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

		for (int i = 0; i < m_collisionShapes.size(); i++)
		{
			btCollisionShape *shape = m_collisionShapes[i];
			delete shape;
		}

		m_collisionShapes.clear();

		delete m_collisionConfiguration;
		delete m_dispatcher;
		delete m_broadphase;
		delete m_solver;
		delete m_dynamicsWorld;
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

		m_dynamicsWorld->stepSimulation(Engine::Get()->GetDelta());
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

	btRigidBody *Scenes::CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
	{
		assert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

		// Rigidbody is dynamic if and only if mass is non zero, otherwise static.
		bool isDynamic = mass != 0.0f;

		btVector3 localInertia(0.0f, 0.0f, 0.0f);

		if (isDynamic)
		{
			shape->calculateLocalInertia(mass, localInertia);
		}

		// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects.
		btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
		btRigidBody *body = new btRigidBody(cInfo);
		//	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
		//	body->setUserIndex(-1);
		return body;
	}
}
