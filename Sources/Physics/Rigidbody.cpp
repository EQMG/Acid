#include "Rigidbody.hpp"

#include <cassert>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include "Scenes/Entity.hpp"
#include "Scenes/Scenes.hpp"
#include "Colliders/Collider.hpp"

namespace acid
{
	Rigidbody::Rigidbody(const float &mass, const float &friction, const Vector3 &linearFactor, const Vector3 &angularFactor) :
		CollisionObject(friction),
		m_mass(mass),
		m_gravity(Vector3::Zero),
		m_linearFactor(linearFactor),
		m_angularFactor(angularFactor),
		m_linearVelocity(Vector3()),
		m_angularVelocity(Vector3()),
		m_rigidBody(nullptr)
	{
	}

	Rigidbody::~Rigidbody()
	{
		btRigidBody *body = btRigidBody::upcast(m_body);

		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}

		auto physics = Scenes::Get()->GetPhysics();

		if (physics != nullptr)
		{
			// FIXME: Are these being deleted?
			physics->GetDynamicsWorld()->removeRigidBody(m_rigidBody);
		}
	}

	void Rigidbody::Start()
	{
		CreateShape();
		auto worldTransform = Collider::Convert(GetParent()->GetWorldTransform());

		m_gravity = Scenes::Get()->GetPhysics()->GetGravity();

		m_motionState = std::make_unique<btDefaultMotionState>(worldTransform);
		m_rigidBody = CreateRigidBody(m_mass, m_motionState.get(), m_shape.get());
		m_rigidBody->setWorldTransform(worldTransform);
		//	m_rigidBody->setContactStiffnessAndDamping(1000.0f, 0.1f);
		m_rigidBody->setFriction(m_friction);
		m_rigidBody->setRollingFriction(m_frictionRolling);
		m_rigidBody->setSpinningFriction(m_frictionSpinning);
		m_rigidBody->setGravity(Collider::Convert(m_gravity));
		m_rigidBody->setLinearFactor(Collider::Convert(m_linearFactor));
		m_rigidBody->setAngularFactor(Collider::Convert(m_angularFactor));
		m_rigidBody->setUserPointer(this);
		m_body = m_rigidBody;
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addRigidBody(m_rigidBody);
		m_rigidBody->activate(true);
		RecalculateMass();
	}

	void Rigidbody::Update()
	{
		if (m_shape.get() != m_body->getCollisionShape())
		{
			m_body->setCollisionShape(m_shape.get());
		}

		for (auto it = m_forces.begin(); it != m_forces.end();)
		{
			(*it)->Update();
			m_rigidBody->applyForce(Collider::Convert((*it)->GetForce()), Collider::Convert((*it)->GetPosition()));

			if ((*it)->IsExpired())
			{
				it = m_forces.erase(it);
				continue;
			}

			++it;
		}

		auto &transform = GetParent()->GetLocalTransform();
		btTransform motionTransform;
		m_rigidBody->getMotionState()->getWorldTransform(motionTransform);
		transform = Collider::Convert(motionTransform, transform.GetScaling());

		m_shape->setLocalScaling(Collider::Convert(transform.GetScaling()));
		//	m_rigidBody->getMotionState()->setWorldTransform(Collider::Convert(transform));
		m_linearVelocity = Collider::Convert(m_rigidBody->getLinearVelocity());
		m_angularVelocity = Collider::Convert(m_rigidBody->getAngularVelocity());
	}

	void Rigidbody::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Friction", m_friction);
		metadata.GetChild("Friction Rolling", m_frictionRolling);
		metadata.GetChild("Friction Spinning", m_frictionSpinning);
		metadata.GetChild("Mass", m_mass);
		metadata.GetChild("Linear Factor", m_linearFactor);
		metadata.GetChild("Angular Factor", m_angularFactor);
	}

	void Rigidbody::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Friction", m_friction);
		metadata.SetChild("Friction Rolling", m_frictionRolling);
		metadata.SetChild("Friction Spinning", m_frictionSpinning);
		metadata.SetChild("Mass", m_mass);
		metadata.SetChild("Linear Factor", m_linearFactor);
		metadata.SetChild("Angular Factor", m_angularFactor);
	}

	bool Rigidbody::InFrustum(const Frustum &frustum)
	{
		btVector3 min = btVector3();
		btVector3 max = btVector3();

		if (m_body != nullptr && m_shape != nullptr)
		{
			m_rigidBody->getAabb(min, max);
		}

		return frustum.CubeInFrustum(Collider::Convert(min), Collider::Convert(max));
	}

	void Rigidbody::ClearForces()
	{
		m_rigidBody->clearForces();
	}

	void Rigidbody::SetMass(const float &mass)
	{
		m_mass = mass;
		RecalculateMass();
	}

	void Rigidbody::SetGravity(const Vector3 &gravity)
	{
		m_gravity = gravity;
		m_rigidBody->setGravity(Collider::Convert(gravity));
	}

	void Rigidbody::SetLinearFactor(const Vector3 &linearFactor)
	{
		m_linearFactor = linearFactor;
		m_rigidBody->setLinearFactor(Collider::Convert(m_linearFactor));
	}

	void Rigidbody::SetAngularFactor(const Vector3 &angularFactor)
	{
		m_angularFactor = angularFactor;
		m_rigidBody->setAngularFactor(Collider::Convert(m_angularFactor));
	}

	void Rigidbody::SetLinearVelocity(const Vector3 &linearVelocity)
	{
		m_linearVelocity = linearVelocity;
		m_rigidBody->setLinearVelocity(Collider::Convert(m_linearVelocity));
	}

	void Rigidbody::SetAngularVelocity(const Vector3 &angularVelocity)
	{
		m_angularVelocity = angularVelocity;
		m_rigidBody->setAngularVelocity(Collider::Convert(m_angularVelocity));
	}

	void Rigidbody::RecalculateMass()
	{
		if (m_rigidBody == nullptr)
		{
			return;
		}

		bool isDynamic = m_mass != 0.0f;

		btVector3 localInertia = btVector3(0.0f, 0.0f, 0.0f);

		auto shape = GetParent()->GetComponent<Collider>();

		if (shape != nullptr && isDynamic)
		{
			shape->GetCollisionShape()->calculateLocalInertia(m_mass, localInertia);
		}

		m_rigidBody->setMassProps(m_mass, localInertia);
	}

	btRigidBody *Rigidbody::CreateRigidBody(float mass, btDefaultMotionState *motionState, btCollisionShape *shape)
	{
		assert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE) && "Invalid rigidbody shape!");

		btVector3 localInertia = btVector3();

		// Rigidbody is dynamic if and only if mass is non zero, otherwise static.
		if (mass != 0.0f)
		{
			shape->calculateLocalInertia(mass, localInertia);
		}

		// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects.
		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, motionState, shape, localInertia);
		auto body = new btRigidBody(cInfo);
		//	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
		return body;
	}
}
