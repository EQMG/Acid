﻿#include "Rigidbody.hpp"

#include <cassert>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include "Scenes/Scenes.hpp"
#include "Collider.hpp"

namespace acid
{
	Rigidbody::Rigidbody(const float &mass, const float &friction, const Transform &localTransform, const Vector3 &linearFactor, const Vector3 &angularFactor) :
		m_mass(mass),
		m_friction(friction),
		m_localTransform(localTransform),
		m_linearFactor(linearFactor),
		m_angularFactor(angularFactor),
		m_shape(nullptr),
		m_body(nullptr),
		m_forces(std::vector<std::unique_ptr<Force>>()),
		m_linearVelocity(Vector3()),
		m_angularVelocity(Vector3())
	{
	}

	Rigidbody::~Rigidbody()
	{
		btRigidBody *body = btRigidBody::upcast(m_body.get());

		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}

		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->removeCollisionObject(m_body.get());
	}

	void Rigidbody::Start()
	{
		btTransform worldTransform = Collider::Convert(GetGameObject()->GetTransform());

		auto shape = GetGameObject()->GetComponent<Collider>();

		if (shape != nullptr && shape->GetCollisionShape() != nullptr)
		{
			m_shape = shape->GetCollisionShape();

			m_body = CreateRigidBody(m_mass, worldTransform, m_shape);
			m_body->setWorldTransform(worldTransform);
		//	m_body->setContactStiffnessAndDamping(1000.0f, 0.1f);
			m_body->setFriction(m_friction);
			m_body->setRollingFriction(m_friction);
			m_body->setSpinningFriction(m_friction);
			// TODO: Set local transform.
			m_body->setLinearFactor(Collider::Convert(m_linearFactor));
			m_body->setAngularFactor(Collider::Convert(m_angularFactor));
			m_body->setUserPointer(GetGameObject());
			Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addRigidBody(m_body.get());
			m_body->activate(true);
		}
	}

	void Rigidbody::Update()
	{
		if (m_body == nullptr)
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
				m_body->setCollisionShape(m_shape);
			}

			if (m_mass != 0.0f)
			{
				btVector3 localInertia = btVector3();
				shape->GetCollisionShape()->calculateLocalInertia(m_mass, localInertia);
				m_body->setMassProps(m_mass, localInertia);
			}
		}

		for (auto it = m_forces.begin(); it != m_forces.end();)
		{
			(*it)->Update();
			m_body->applyForce(Collider::Convert((*it)->GetForce()), Collider::Convert((*it)->GetPosition()));

			if ((*it)->IsExpired())
			{
				it = m_forces.erase(it);
				continue;
			}

			++it;
		}

		auto &transform = GetGameObject()->GetTransform();
		transform = Collider::Convert(m_body->getWorldTransform(), transform.GetScaling());

		m_shape->setLocalScaling(Collider::Convert(m_localTransform.GetScaling() * transform.GetScaling()));
	//  m_body->getMotionState()->setWorldTransform(*worldTransform);
		m_linearVelocity = Collider::Convert(m_body->getLinearVelocity());
		m_angularVelocity = Collider::Convert(m_body->getAngularVelocity());
	//	m_body->setLinearVelocity(Collider::Convert(m_linearVelocity));
	//	m_body->setAngularVelocity(Collider::Convert(m_angularVelocity));
	}

	void Rigidbody::Decode(const Metadata &metadata)
	{
		m_mass = metadata.GetChild<float>("Mass");
		m_friction = metadata.GetChild<float>("Friction");
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
		m_linearFactor = metadata.GetChild<Vector3>("Linear Factor");
		m_angularFactor = metadata.GetChild<Vector3>("Angular Factor");
	}

	void Rigidbody::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("Mass", m_mass);
		metadata.SetChild<float>("Friction", m_friction);
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
		metadata.SetChild<Vector3>("Linear Factor", m_linearFactor);
		metadata.SetChild<Vector3>("Angular Factor", m_angularFactor);
	}

	bool Rigidbody::InFrustum(const Frustum &frustum)
	{
		btVector3 min = btVector3();
		btVector3 max = btVector3();

		if (m_body != nullptr && m_shape != nullptr)
		{
			m_body->getAabb(min, max);
		}

		return frustum.CubeInFrustum(Collider::Convert(min), Collider::Convert(max));
	}

	void Rigidbody::SetGravity(const Vector3 &gravity)
	{
		m_body->setGravity(Collider::Convert(gravity));
	}

	Force *Rigidbody::AddForce(Force *force)
	{
		m_forces.emplace_back(force);
		return force;
	}

	void Rigidbody::ClearForces()
	{
		m_body->clearForces();
	}

	void Rigidbody::SetMass(const float &mass)
	{
		m_mass = mass;

		bool isDynamic = m_mass != 0.0f;

		btVector3 localInertia = btVector3(0.0f, 0.0f, 0.0f);

		auto shape = GetGameObject()->GetComponent<Collider>();

		if (shape != nullptr && isDynamic)
		{
			shape->GetCollisionShape()->calculateLocalInertia(m_mass, localInertia);
		}

		m_body->setMassProps(m_mass, localInertia);
	}

	void Rigidbody::SetFriction(const float &friction)
	{
		m_friction = friction;
		m_body->setFriction(m_friction);
		m_body->setRollingFriction(m_friction);
		m_body->setSpinningFriction(m_friction);
	}

	void Rigidbody::SetLocalTransform(const Transform &localTransform)
	{
		m_localTransform = localTransform;
		// TODO: Set local transform.
	}

	void Rigidbody::SetLinearFactor(const Vector3 &linearFactor)
	{
		m_linearFactor = linearFactor;
		m_body->setLinearFactor(Collider::Convert(m_linearFactor));
	}

	void Rigidbody::SetAngularFactor(const Vector3 &angularFactor)
	{
		m_angularFactor = angularFactor;
		m_body->setAngularFactor(Collider::Convert(m_angularFactor));
	}

	void Rigidbody::SetLinearVelocity(const Vector3 &linearVelocity)
	{
		m_linearVelocity = linearVelocity;
		m_body->setLinearVelocity(Collider::Convert(m_linearVelocity));
	}

	void Rigidbody::SetAngularVelocity(const Vector3 &angularVelocity)
	{
		m_angularVelocity = angularVelocity;
		m_body->setAngularVelocity(Collider::Convert(m_angularVelocity));
	}

	std::unique_ptr<btRigidBody> Rigidbody::CreateRigidBody(float mass, const btTransform &startTransform, btCollisionShape* shape)
	{
		assert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE) && "Invalid rigidbody shape!");

		btVector3 localInertia = btVector3();

		// Rigidbody is dynamic if and only if mass is non zero, otherwise static.
		if (mass != 0.0f)
		{
			shape->calculateLocalInertia(mass, localInertia);
		}

		// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects.
		btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
		auto body = std::make_unique<btRigidBody>(cInfo);
	//	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
	//	body->setUserIndex(-1);
		return body;
	}
}
