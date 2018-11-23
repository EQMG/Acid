#include "KinematicCharacter.hpp"

#include <cassert>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	KinematicCharacter::KinematicCharacter(const float &friction) :
		CollisionObject(friction),
		m_controller(nullptr),
		m_gravity(Vector3::ZERO),
		m_up(Vector3::UP),
		m_stepHeight(0.0f),
		m_fallSpeed(55.0f),
		m_jumpSpeed(10.0f),
		m_maxHeight(1.5f),
		m_interpolate(true),
		m_ghostObject(nullptr)
	{
	}

	KinematicCharacter::~KinematicCharacter()
	{
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->removeAction(m_controller.get());
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->removeCollisionObject(m_ghostObject);
	}

	void KinematicCharacter::Start()
	{
		CreateShape();
		btTransform worldTransform = Collider::Convert(GetGameObject()->GetTransform());

		m_gravity = Scenes::Get()->GetPhysics()->GetGravity();

		m_body = CreateGhostObject(1.0f, worldTransform, m_shape.get());
		m_ghostObject = dynamic_cast<btPairCachingGhostObject *>(m_body.get());
		m_ghostObject->setFriction(m_friction);
		m_ghostObject->setRollingFriction(m_frictionRolling);
		m_ghostObject->setSpinningFriction(m_frictionSpinning);
		m_ghostObject->setUserPointer(this);

		m_controller = std::make_unique<btKinematicCharacterController>(m_ghostObject, dynamic_cast<btConvexShape *>(m_shape.get()), 0.01666f);
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addAction(m_controller.get());
		m_controller->setGravity(Collider::Convert(m_gravity));
		m_controller->setUp(Collider::Convert(m_up));
		m_controller->setStepHeight(m_stepHeight);
		m_controller->setFallSpeed(m_fallSpeed);
		m_controller->setJumpSpeed(m_jumpSpeed);
		m_controller->setMaxJumpHeight(m_maxHeight);
		m_controller->setUpInterpolate(m_interpolate);
	}

	void KinematicCharacter::Update()
	{
		if (m_shape.get() != m_body->getCollisionShape())
		{
			m_body->setCollisionShape(m_shape.get());
		}

		auto &transform = GetGameObject()->GetTransform();
		btTransform worldTransform = m_ghostObject->getWorldTransform();
		transform = Collider::Convert(worldTransform, transform.GetScaling());
	}

	void KinematicCharacter::Decode(const Metadata &metadata)
	{
		m_friction = metadata.GetChild<float>("Friction");
		m_frictionRolling = metadata.GetChild<float>("Friction Rolling");
		m_frictionSpinning = metadata.GetChild<float>("Friction Spinning");
		m_up = metadata.GetChild<Vector3>("Up");
		m_stepHeight = metadata.GetChild<float>("Height");
		m_fallSpeed = metadata.GetChild<float>("Fall Speed");
		m_jumpSpeed = metadata.GetChild<float>("Jump Speed");
		m_maxHeight = metadata.GetChild<float>("Max Height");
		m_interpolate = metadata.GetChild<bool>("Interpolate");
	}

	void KinematicCharacter::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("Friction", m_friction);
		metadata.SetChild<float>("Friction Rolling", m_frictionRolling);
		metadata.SetChild<float>("Friction Spinning", m_frictionSpinning);
		metadata.SetChild<Vector3>("Up", m_up);
		metadata.SetChild<float>("Step Height", m_stepHeight);
		metadata.SetChild<float>("Fall Speed", m_fallSpeed);
		metadata.SetChild<float>("Jump Speed", m_jumpSpeed);
		metadata.SetChild<float>("Max Height", m_maxHeight);
		metadata.SetChild<bool>("Interpolate", m_interpolate);
	}

	bool KinematicCharacter::InFrustum(const Frustum &frustum)
	{
		btVector3 min = btVector3();
		btVector3 max = btVector3();

		if (m_body != nullptr && m_shape != nullptr)
		{
			m_shape->getAabb(Collider::Convert(GetGameObject()->GetTransform()), min, max);
		}

		return frustum.CubeInFrustum(Collider::Convert(min), Collider::Convert(max));
	}

	void KinematicCharacter::ClearForces()
	{
	//	m_controller->clearForces();
	}

	void KinematicCharacter::SetGravity(const Vector3 &gravity)
	{
		m_gravity = gravity;
		m_controller->setGravity(Collider::Convert(gravity));
	}

	void KinematicCharacter::SetUp(const Vector3 &up)
	{
		m_up = up;
		m_controller->setUp(Collider::Convert(up));
	}

	void KinematicCharacter::SetStepHeight(const float &stepHeight)
	{
		m_stepHeight = stepHeight;
		m_controller->setStepHeight(stepHeight);
	}

	void KinematicCharacter::SetFallSpeed(const float &fallSpeed)
	{
		m_fallSpeed = fallSpeed;
		m_controller->setFallSpeed(fallSpeed);
	}

	void KinematicCharacter::SetJumpSpeed(const float &jumpSpeed)
	{
		m_jumpSpeed = jumpSpeed;
		m_controller->setJumpSpeed(jumpSpeed);
	}

	void KinematicCharacter::SetMaxJumpHeight(const float &maxHeight)
	{
		m_maxHeight = maxHeight;
		m_controller->setMaxJumpHeight(maxHeight);
	}

	void KinematicCharacter::SetInterpolate(const bool &interpolate)
	{
		m_interpolate = interpolate;
		m_controller->setUpInterpolate(interpolate);
	}

	bool KinematicCharacter::IsOnGround() const
	{
		return m_controller->onGround();
	}

	void KinematicCharacter::Jump(const Vector3 &direction)
	{
		m_controller->jump(Collider::Convert(direction));
	}

	void KinematicCharacter::SetWalkDirection(const Vector3 &direction)
	{
		m_controller->setWalkDirection(Collider::Convert(direction));
	}

	void KinematicCharacter::RecalculateMass()
	{
	}

	std::unique_ptr<btPairCachingGhostObject> KinematicCharacter::CreateGhostObject(float mass, const btTransform &startTransform, btCollisionShape *shape)
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
	}
}
