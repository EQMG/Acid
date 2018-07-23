#include "Rigidbody.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include "Scenes/Scenes.hpp"
#include "ICollider.hpp"

namespace fl
{
	Rigidbody::Rigidbody(const float &mass, const float &friction, const Vector3 &linearFactor, const Vector3 &angularFactor) :
		IComponent(),
		m_mass(mass),
		m_friction(friction),
		m_linearFactor(linearFactor),
		m_angularFactor(angularFactor),
		m_worldTransform(btTransform()),
		m_shape(nullptr),
		m_body(nullptr)
	{
	}

	Rigidbody::~Rigidbody()
	{
		btRigidBody *body = btRigidBody::upcast(m_body);

		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}

		Scenes::Get()->GetDynamicsWorld()->removeCollisionObject(m_body);

		delete m_body;
	}

	void Rigidbody::Start()
	{
		Vector3 position = GetGameObject()->GetTransform().GetPosition();
		Quaternion rotation = GetGameObject()->GetTransform().GetRotation();

		m_worldTransform.setIdentity();
		m_worldTransform.setOrigin(ICollider::Convert(position));
		m_worldTransform.setRotation(ICollider::Convert(rotation));

		auto shape = GetGameObject()->GetComponent<ICollider>();

		if (shape != nullptr && shape->GetCollisionShape() != nullptr)
		{
			m_shape = shape->GetCollisionShape();

			m_body = CreateRigidBody(m_mass, m_worldTransform, m_shape);
			m_body->setWorldTransform(m_worldTransform);
			m_body->setContactStiffnessAndDamping(1000.0f, 0.1f);
			m_body->setFriction(m_friction);
			m_body->setRollingFriction(m_friction);
			m_body->setSpinningFriction(m_friction);
			m_body->setLinearFactor(ICollider::Convert(m_linearFactor));
			m_body->setAngularFactor(ICollider::Convert(m_angularFactor));
			m_body->setUserPointer(GetGameObject());
			Scenes::Get()->GetDynamicsWorld()->addRigidBody(m_body);
		}
	}

	void Rigidbody::Update()
	{
		if (m_body == nullptr)
		{
			Start();
			return;
		}

		auto shape = GetGameObject()->GetComponent<ICollider>();

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

		auto &transform = GetGameObject()->GetTransform();

		if (m_linearFactor != Vector3::ZERO)
		{
			btVector3 position = m_body->getWorldTransform().getOrigin();
			transform.SetPosition(ICollider::Convert(position));
		}

		if (m_angularFactor != Vector3::ZERO)
		{
			btQuaternion rotation = m_body->getWorldTransform().getRotation();
			transform.SetRotation(ICollider::Convert(rotation));
		}

	//	m_worldTransform.setIdentity();
	//	m_worldTransform.setOrigin(ICollider::Convert(transform.GetPosition()));
	//	m_worldTransform.setRotation(ICollider::Convert(transform.GetRotation()));

		m_shape->setLocalScaling(ICollider::Convert(transform.GetScaling()));
	//	m_body->setWorldTransform(m_worldTransform);
	//	m_body->setLinearVelocity(m_velocity);
	}

	void Rigidbody::Load(LoadedValue *value)
	{
		m_mass = value->GetChild("Mass")->Get<float>();
		m_friction = value->GetChild("Friction")->Get<float>();
		m_linearFactor = value->GetChild("Linear Factor");
		m_angularFactor = value->GetChild("Angular Factor");
	}

	void Rigidbody::Write(LoadedValue *destination)
	{
		destination->GetChild("Mass", true)->Set(m_mass);
		destination->GetChild("Friction", true)->Set(m_friction);
		m_linearFactor.Write(destination->GetChild("Linear Factor", true));
		m_angularFactor.Write(destination->GetChild("Angular Factor", true));
	}

	void Rigidbody::SetAngularVelocity(const Vector3 &velocity)
	{
		m_body->setAngularVelocity(ICollider::Convert(velocity));
	}

	void Rigidbody::SetLinearVelocity(const Vector3 &velocity)
	{
		m_body->setLinearVelocity(ICollider::Convert(velocity));
	}

	void Rigidbody::AddForce(const Vector3 &force, const Vector3 &position)
	{
		if (m_body == nullptr)
		{
			return;
		}

		m_body->applyForce(ICollider::Convert(force), ICollider::Convert(position));
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

		auto shape = GetGameObject()->GetComponent<ICollider>();

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
		m_body->setLinearFactor(ICollider::Convert(m_linearFactor));
	}

	void Rigidbody::SetAngularFactor(const Vector3 &angularFactor)
	{
		m_angularFactor = angularFactor;
		m_body->setAngularFactor(ICollider::Convert(m_angularFactor));
	}

	btRigidBody *Rigidbody::CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
	{
		assert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

		btVector3 localInertia = btVector3();

		// Rigidbody is dynamic if and only if mass is non zero, otherwise static.
		if (mass != 0.0f)
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
