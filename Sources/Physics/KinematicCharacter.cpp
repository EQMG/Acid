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
		m_gravity(Vector3::Zero),
		m_up(Vector3::Up),
		m_stepHeight(0.0f),
		m_fallSpeed(55.0f),
		m_jumpSpeed(10.0f),
		m_maxHeight(1.5f),
		m_interpolate(true),
		m_ghostObject(nullptr),
		m_controller(nullptr)
	{
	}

	KinematicCharacter::~KinematicCharacter()
	{
		auto physics = Scenes::Get()->GetPhysics();

		if (physics != nullptr)
		{
			// FIXME: Are these being deleted?
			physics->GetDynamicsWorld()->removeCollisionObject(m_ghostObject);
			physics->GetDynamicsWorld()->removeAction(m_controller);
		}
	}

	void KinematicCharacter::Start()
	{
		CreateShape(true);
		auto worldTransform = Collider::Convert(GetParent()->GetWorldTransform());

		m_gravity = Scenes::Get()->GetPhysics()->GetGravity();

		m_ghostObject = CreateGhostObject(1.0f, worldTransform, m_shape.get());
		m_ghostObject->setFriction(m_friction);
		m_ghostObject->setRollingFriction(m_frictionRolling);
		m_ghostObject->setSpinningFriction(m_frictionSpinning);
		m_ghostObject->setUserPointer(this);
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
		m_body = m_ghostObject;

		m_controller = new btKinematicCharacterController(m_ghostObject, static_cast<btConvexShape *>(m_shape.get()), 0.03f);
		m_controller->setGravity(Collider::Convert(m_gravity));
		m_controller->setUp(Collider::Convert(m_up));
		m_controller->setStepHeight(m_stepHeight);
		m_controller->setFallSpeed(m_fallSpeed);
		m_controller->setJumpSpeed(m_jumpSpeed);
		m_controller->setMaxJumpHeight(m_maxHeight);
		m_controller->setUpInterpolate(m_interpolate);
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addAction(m_controller);
	}

	void KinematicCharacter::Update()
	{
		if (m_shape.get() != m_body->getCollisionShape())
		{
			m_body->setCollisionShape(m_shape.get());
		}

		auto &transform = GetParent()->GetLocalTransform();
		btTransform worldTransform = m_ghostObject->getWorldTransform();
		transform = Collider::Convert(worldTransform, transform.GetScaling());
	}

	void KinematicCharacter::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Friction", m_friction);
		metadata.GetChild("Friction Rolling", m_frictionRolling);
		metadata.GetChild("Friction Spinning", m_frictionSpinning);
		metadata.GetChild("Up", m_up);
		metadata.GetChild("Step Height", m_stepHeight);
		metadata.GetChild("Fall Speed", m_fallSpeed);
		metadata.GetChild("Jump Speed", m_jumpSpeed);
		metadata.GetChild("Max Height", m_maxHeight);
		metadata.GetChild("Interpolate", m_interpolate);
	}

	void KinematicCharacter::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Friction", m_friction);
		metadata.SetChild("Friction Rolling", m_frictionRolling);
		metadata.SetChild("Friction Spinning", m_frictionSpinning);
		metadata.SetChild("Up", m_up);
		metadata.SetChild("Step Height", m_stepHeight);
		metadata.SetChild("Fall Speed", m_fallSpeed);
		metadata.SetChild("Jump Speed", m_jumpSpeed);
		metadata.SetChild("Max Height", m_maxHeight);
		metadata.SetChild("Interpolate", m_interpolate);
	}

	bool KinematicCharacter::InFrustum(const Frustum &frustum)
	{
		btVector3 min = btVector3();
		btVector3 max = btVector3();

		if (m_body != nullptr && m_shape != nullptr)
		{
			m_shape->getAabb(Collider::Convert(GetParent()->GetWorldTransform()), min, max);
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

	btPairCachingGhostObject *KinematicCharacter::CreateGhostObject(float mass, const btTransform &startTransform, btCollisionShape *shape)
	{
		assert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE) && "Invalid ghost object shape!");

		btVector3 localInertia = btVector3();

		// Rigidbody is dynamic if and only if mass is non zero, otherwise static.
		if (mass != 0.0f)
		{
			shape->calculateLocalInertia(mass, localInertia);
		}

		auto ghostObject = new btPairCachingGhostObject();
		ghostObject->setWorldTransform(startTransform);
		Scenes::Get()->GetPhysics()->GetBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		ghostObject->setCollisionShape(shape);
		ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		return ghostObject;
	}
}
