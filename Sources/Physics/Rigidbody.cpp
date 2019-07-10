#include "Rigidbody.hpp"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include "Maths/Transform.hpp"
#include "Scenes/Entity.hpp"
#include "Scenes/Scenes.hpp"
#include "Colliders/Collider.hpp"

namespace acid
{
Rigidbody::Rigidbody(const float &mass, const float &friction, const Vector3f &linearFactor, const Vector3f &angularFactor) :
	CollisionObject{mass, friction, linearFactor, angularFactor}
{
}

Rigidbody::~Rigidbody()
{
	auto body{btRigidBody::upcast(m_body)};

	if (body != nullptr && body->getMotionState() != nullptr)
	{
		delete body->getMotionState();
	}

	auto physics{Scenes::Get()->GetPhysics()};

	if (physics != nullptr)
	{
		physics->GetDynamicsWorld()->removeRigidBody(m_rigidBody.get());
	}
}

void Rigidbody::Start()
{
	if (m_rigidBody != nullptr)
	{
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->removeRigidBody(m_rigidBody.get());
	}

	CreateShape();
	assert((m_shape == nullptr || m_shape->getShapeType() != INVALID_SHAPE_PROXYTYPE) && "Invalid rigidbody shape!");
	m_gravity = Scenes::Get()->GetPhysics()->GetGravity();
	btVector3 localInertia;

	// Rigidbody is dynamic if and only if mass is non zero, otherwise static.
	if (m_mass != 0.0f)
	{
		m_shape->calculateLocalInertia(m_mass, localInertia);
	}

	auto worldTransform{Collider::Convert(*GetEntity()->GetComponent<Transform>())};

	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects.
	auto motionState{new btDefaultMotionState{worldTransform}};
	btRigidBody::btRigidBodyConstructionInfo cInfo{m_mass, motionState, m_shape.get(), localInertia};

	m_rigidBody = std::make_unique<btRigidBody>(cInfo);
	//m_rigidBody->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
	m_rigidBody->setWorldTransform(worldTransform);
	//m_rigidBody->setContactStiffnessAndDamping(1000.0f, 0.1f);
	m_rigidBody->setFriction(m_friction);
	m_rigidBody->setRollingFriction(m_frictionRolling);
	m_rigidBody->setSpinningFriction(m_frictionSpinning);
	m_rigidBody->setGravity(Collider::Convert(m_gravity));
	m_rigidBody->setLinearFactor(Collider::Convert(m_linearFactor));
	m_rigidBody->setAngularFactor(Collider::Convert(m_angularFactor));
	m_rigidBody->setUserPointer(this);
	m_body = m_rigidBody.get();
	Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addRigidBody(m_rigidBody.get());
	m_rigidBody->activate(true);
	RecalculateMass();
}

void Rigidbody::Update()
{
	if (m_shape.get() != m_body->getCollisionShape())
	{
		m_body->setCollisionShape(m_shape.get());
	}

	auto delta{Engine::Get()->GetDelta()};

	for (auto it{m_forces.begin()}; it != m_forces.end();)
	{
		(*it)->Update(delta);
		m_rigidBody->applyForce(Collider::Convert((*it)->GetForce()), Collider::Convert((*it)->GetPosition()));

		if ((*it)->IsExpired())
		{
			it = m_forces.erase(it);
			continue;
		}

		++it;
	}

	auto &transform{*GetEntity()->GetComponent<Transform>()};
	btTransform motionTransform;
	m_rigidBody->getMotionState()->getWorldTransform(motionTransform);
	transform = Collider::Convert(motionTransform, transform.GetScale());

	m_shape->setLocalScaling(Collider::Convert(transform.GetScale()));
	//m_rigidBody->getMotionState()->setWorldTransform(Collider::Convert(transform));
	m_linearVelocity = Collider::Convert(m_rigidBody->getLinearVelocity());
	m_angularVelocity = Collider::Convert(m_rigidBody->getAngularVelocity());
}

bool Rigidbody::InFrustum(const Frustum &frustum)
{
	btVector3 min;
	btVector3 max;

	if (m_body != nullptr && m_shape != nullptr)
	{
		m_rigidBody->getAabb(min, max);
	}

	return frustum.CubeInFrustum(Collider::Convert(min), Collider::Convert(max));
}

void Rigidbody::ClearForces()
{
	if (m_rigidBody != nullptr)
	{
		m_rigidBody->clearForces();
	}
}

void Rigidbody::SetMass(const float &mass)
{
	m_mass = mass;
	RecalculateMass();
}

void Rigidbody::SetGravity(const Vector3f &gravity)
{
	m_gravity = gravity;

	if (m_rigidBody != nullptr)
	{
		m_rigidBody->setGravity(Collider::Convert(gravity));
	}
}

void Rigidbody::SetLinearFactor(const Vector3f &linearFactor)
{
	m_linearFactor = linearFactor;

	if (m_rigidBody != nullptr)
	{
		m_rigidBody->setLinearFactor(Collider::Convert(m_linearFactor));
	}
}

void Rigidbody::SetAngularFactor(const Vector3f &angularFactor)
{
	m_angularFactor = angularFactor;

	if (m_rigidBody != nullptr)
	{
		m_rigidBody->setAngularFactor(Collider::Convert(m_angularFactor));
	}
}

void Rigidbody::SetLinearVelocity(const Vector3f &linearVelocity)
{
	m_linearVelocity = linearVelocity;

	if (m_rigidBody != nullptr)
	{
		m_rigidBody->setLinearVelocity(Collider::Convert(m_linearVelocity));
	}
}

void Rigidbody::SetAngularVelocity(const Vector3f &angularVelocity)
{
	m_angularVelocity = angularVelocity;

	if (m_rigidBody != nullptr)
	{
		m_rigidBody->setAngularVelocity(Collider::Convert(m_angularVelocity));
	}
}

const Node &operator>>(const Node &node, Rigidbody &rigidbody)
{
	node["mass"].Get(rigidbody.m_mass);
	node["friction"].Get(rigidbody.m_friction);
	node["frictionRolling"].Get(rigidbody.m_frictionRolling);
	node["frictionSpinning"].Get(rigidbody.m_frictionSpinning);
	node["linearFactor"].Get(rigidbody.m_linearFactor);
	node["angularFactor"].Get(rigidbody.m_angularFactor);
	return node;
}

Node &operator<<(Node &node, const Rigidbody &rigidbody)
{
	node["mass"].Set(rigidbody.m_mass);
	node["friction"].Set(rigidbody.m_friction);
	node["frictionRolling"].Set(rigidbody.m_frictionRolling);
	node["frictionSpinning"].Set(rigidbody.m_frictionSpinning);
	node["linearFactor"].Set(rigidbody.m_linearFactor);
	node["angularFactor"].Set(rigidbody.m_angularFactor);
	return node;
}

void Rigidbody::RecalculateMass()
{
	if (m_rigidBody == nullptr)
	{
		return;
	}

	auto isDynamic{m_mass != 0.0f};

	btVector3 localInertia;

	auto shape{GetEntity()->GetComponent<Collider>()};

	if (shape != nullptr && isDynamic)
	{
		shape->GetCollisionShape()->calculateLocalInertia(m_mass, localInertia);
	}

	m_rigidBody->setMassProps(m_mass, localInertia);
}
}
