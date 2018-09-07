#include "Rigidbody.hpp"

#include <cassert>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include "Scenes/Scenes.hpp"
#include "Collider.hpp"

namespace acid
{
	Rigidbody::Rigidbody(const float &mass, const float &friction, const Vector3 &linearFactor, const Vector3 &angularFactor) :
		m_mass(mass),
		m_friction(friction),
		m_linearFactor(linearFactor),
		m_angularFactor(angularFactor),
		m_shape(nullptr),
		m_body(nullptr),
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
		Vector3 position = GetGameObject()->GetTransform().GetPosition();
		Quaternion rotation = GetGameObject()->GetTransform().GetRotation();

		btTransform worldTransform = btTransform();
		worldTransform.setIdentity();
		worldTransform.setOrigin(Collider::Convert(position));
		worldTransform.setRotation(Collider::Convert(rotation));

		auto shape = GetGameObject()->GetComponent<Collider>();

		if (shape != nullptr && shape->GetCollisionShape() != nullptr)
		{
			m_shape = shape->GetCollisionShape();

			m_body.reset(CreateRigidBody(m_mass, worldTransform, m_shape));
			m_body->setWorldTransform(worldTransform);
		//	m_body->setContactStiffnessAndDamping(1000.0f, 0.1f);
			m_body->setFriction(m_friction);
			m_body->setRollingFriction(m_friction);
			m_body->setSpinningFriction(m_friction);
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
		btTransform worldTransform;
		m_body->getMotionState()->getWorldTransform(worldTransform);

		if (m_linearFactor != Vector3::ZERO)
		{
			btVector3 position = worldTransform.getOrigin();
			transform.SetPosition(Collider::Convert(position));
		}

		if (m_angularFactor != Vector3::ZERO)
		{
			btQuaternion rotation = worldTransform.getRotation();
		//	transform.SetRotation(Collider::Convert(rotation));
			float yaw, pitch, roll;
			rotation.getEulerZYX(yaw, pitch, roll);
			transform.SetRotation(Vector3(pitch * RAD_TO_DEG, yaw * RAD_TO_DEG, roll * RAD_TO_DEG));
		}

	//  worldTransform->setIdentity();
	//  worldTransform->setOrigin(Collider::Convert(transform.GetPosition()));
	//  worldTransform->setRotation(Collider::Convert(transform.GetRotation()));

		m_shape->setLocalScaling(Collider::Convert(transform.GetScaling()));
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
		m_linearFactor = metadata.GetChild<Vector3>("Linear Factor");
		m_angularFactor = metadata.GetChild<Vector3>("Angular Factor");
	}

	void Rigidbody::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("Mass", m_mass);
		metadata.SetChild<float>("Friction", m_friction);
		metadata.SetChild<Vector3>("Linear Factor", m_linearFactor);
		metadata.SetChild<Vector3>("Angular Factor", m_angularFactor);
	}

	void Rigidbody::SetGravity(const Vector3 &gravity)
	{
		m_body->setGravity(Collider::Convert(gravity));
	}

	std::shared_ptr<Force> Rigidbody::AddForce(const std::shared_ptr<Force> &force)
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

		btVector3 localInertia(0.0f, 0.0f, 0.0f);

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

	btRigidBody* Rigidbody::CreateRigidBody(float mass, const btTransform &startTransform, btCollisionShape* shape)
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
		auto body = new btRigidBody(cInfo);
	//	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
	//	body->setUserIndex(-1);
		return body;
	}
}
