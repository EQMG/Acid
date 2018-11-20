#include "CharacterController.hpp"

#include <cassert>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	CharacterController::CharacterController() :
		m_shape(nullptr),
		m_ghostObject(nullptr),
		m_controller(nullptr)
	{
	}

	CharacterController::~CharacterController()
	{
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->removeAction(m_controller.get());
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->removeCollisionObject(m_ghostObject.get());
	}

	void CharacterController::Start()
	{
		float m_mass = 1.0f;
		btTransform worldTransform = Collider::Convert(GetGameObject()->GetTransform());

		auto shape = GetGameObject()->GetComponent<Collider>();

		if (shape != nullptr && shape->GetCollisionShape() != nullptr)
		{
			m_shape = shape->GetCollisionShape();

			m_ghostObject = CreateGhostObject(m_mass, worldTransform, m_shape);

			m_controller = std::make_unique<btKinematicCharacterController>(m_ghostObject.get(), (btConvexShape *)shape->GetCollisionShape(), 0.01666f);
			Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addCollisionObject(m_ghostObject.get(), btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
			Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addAction(m_controller.get());
			m_controller->setMaxJumpHeight(1.5f);
			m_controller->setUp(btVector3(0.0f, 1.0f, 0.0f));
			m_controller->setGravity(Collider::Convert(Scenes::Get()->GetPhysics()->GetGravity()));
		}
	}

	void CharacterController::Update()
	{
		if (m_controller == nullptr)
		{
			Start();
			return;
		}

		auto shape = GetGameObject()->GetComponent<Collider>();

		if (shape != nullptr)
		{
			if (m_shape != shape->GetCollisionShape())
			{
				m_shape = shape->GetCollisionShape();
			//	m_controller->setCollisionShape(m_shape);
			}

			/*if (m_mass != 0.0f)
			{
				btVector3 localInertia = btVector3();
				shape->GetCollisionShape()->calculateLocalInertia(m_mass, localInertia);
				m_body->setMassProps(m_mass, localInertia);
			}*/
		}

		auto &transform = GetGameObject()->GetTransform();
		btTransform worldTransform = m_ghostObject->getWorldTransform();
		transform = Collider::Convert(worldTransform, transform.GetScaling());
	}

	void CharacterController::Decode(const Metadata &metadata)
	{
	}

	void CharacterController::Encode(Metadata &metadata) const
	{
	}

	void CharacterController::SetGravity(const Vector3 &gravity)
	{
		if (m_controller == nullptr)
		{
			return;
		}

		m_controller->setGravity(Collider::Convert(gravity));
	}

	void CharacterController::SetUp(const Vector3 &up)
	{
		if (m_controller == nullptr)
		{
			return;
		}

		m_controller->setUp(Collider::Convert(up));
	}

	void CharacterController::SetStepHeight(const float &stepHeight)
	{
		if (m_controller == nullptr)
		{
			return;
		}

		m_controller->setStepHeight(stepHeight);
	}

	void CharacterController::SetFallSpeed(const float &fallSpeed)
	{
		if (m_controller == nullptr)
		{
			return;
		}

		m_controller->setFallSpeed(fallSpeed);
	}

	void CharacterController::SetJumpSpeed(const float &jumpSpeed)
	{
		if (m_controller == nullptr)
		{
			return;
		}

		m_controller->setJumpSpeed(jumpSpeed);
	}

	void CharacterController::SetMaxJumpHeight(const float &maxHeight)
	{
		if (m_controller == nullptr)
		{
			return;
		}

		m_controller->setMaxJumpHeight(maxHeight);
	}

	void CharacterController::SetInterpolate(const bool &interpolate)
	{
		if (m_controller == nullptr)
		{
			return;
		}

		m_controller->setUpInterpolate(interpolate);
	}

	bool CharacterController::IsOnGround() const
	{
		if (m_controller == nullptr)
		{
			return false;
		}

		return m_controller->onGround();
	}

	void CharacterController::Jump(const Vector3 &direction)
	{
		if (m_controller == nullptr)
		{
			return;
		}

		m_controller->jump(Collider::Convert(direction));
	}

	void CharacterController::SetWalkDirection(const Vector3 &direction)
	{
		if (m_controller == nullptr)
		{
			return;
		}

		m_controller->setWalkDirection(Collider::Convert(direction));
	}

	std::unique_ptr<btPairCachingGhostObject> CharacterController::CreateGhostObject(float mass, const btTransform &startTransform, btCollisionShape *shape)
	{
		assert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE) && "Invalid ghost object shape!");

		btVector3 localInertia = btVector3();

		// Rigidbody is dynamic if and only if mass is non zero, otherwise static.
		if (mass != 0.0f)
		{
			shape->calculateLocalInertia(mass, localInertia);
		}

		auto ghostObject = std::make_unique<btPairCachingGhostObject>();
		ghostObject->setWorldTransform(startTransform);
		Scenes::Get()->GetPhysics()->GetBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		ghostObject->setCollisionShape(shape);
		ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		return ghostObject;

		/*// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects.
		btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
		auto body = std::make_unique<btRigidBody>(cInfo);
		//	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
		//	body->setUserIndex(-1);
		return body;*/
	}
}
