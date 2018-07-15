#include "Rigidbody.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include "Scenes/Scenes.hpp"
#include "IShape.hpp"

namespace fl
{
	Rigidbody::Rigidbody(const float &mass, const float &stiffness, const float &damping, const Constraint3 &freezePosition, const Constraint3 &freezeRotation) :
		IComponent(),
		m_mass(mass),
		m_stiffness(stiffness),
		m_damping(damping),
		m_freezePosition(freezePosition),
		m_freezeRotation(freezeRotation),
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
		m_worldTransform.setOrigin(IShape::Convert(position));
		m_worldTransform.setRotation(IShape::Convert(rotation));

		auto shape = GetGameObject()->GetComponent<IShape>();

		if (shape != nullptr && shape->GetCollisionShape() != nullptr)
		{
			m_shape = shape->GetCollisionShape();

			m_body = CreateRigidBody(m_mass, m_worldTransform, m_shape);
			m_body->setWorldTransform(m_worldTransform);
			m_body->setContactStiffnessAndDamping(m_stiffness, m_damping);
			m_body->setLinearFactor(IShape::Convert(m_freezePosition, true));
			m_body->setAngularFactor(IShape::Convert(m_freezeRotation, true));
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

		auto shape = GetGameObject()->GetComponent<IShape>();

		if (shape != nullptr && m_shape != shape->GetCollisionShape())
		{
			m_shape = shape->GetCollisionShape();
			m_body->setCollisionShape(shape->GetCollisionShape());
		}

		auto &transform = GetGameObject()->GetTransform();

		if (m_freezePosition != Constraint3::ONE)
		{
			btVector3 position = m_body->getWorldTransform().getOrigin();
			transform.SetPosition(IShape::Convert(position));
		}

		if (m_freezeRotation != Constraint3::ONE)
		{
			btQuaternion rotation = m_body->getWorldTransform().getRotation();
			transform.SetRotation(IShape::Convert(rotation));
		}

	//	m_worldTransform.setIdentity();
	//	m_worldTransform.setOrigin(IShape::Convert(transform.GetPosition()));
	//	m_worldTransform.setRotation(IShape::Convert(transform.GetRotation()));

		m_shape->setLocalScaling(IShape::Convert(transform.GetScaling()));
	//	m_body->setWorldTransform(m_worldTransform);
	//	m_body->setLinearVelocity(m_velocity);
	}

	void Rigidbody::Load(LoadedValue *value)
	{
		m_mass = value->GetChild("Mass")->Get<float>();
		m_stiffness = value->GetChild("Stiffness")->Get<float>();
		m_damping = value->GetChild("Damping")->Get<float>();
		m_freezePosition = value->GetChild("Freeze Position");
		m_freezeRotation = value->GetChild("Freeze Rotation");
	}

	void Rigidbody::Write(LoadedValue *destination)
	{
		destination->GetChild("Mass", true)->Set(m_mass);
		destination->GetChild("Stiffness", true)->Set(m_stiffness);
		destination->GetChild("Damping", true)->Set(m_damping);
		m_freezePosition.Write(destination->GetChild("Freeze Position", true));
		m_freezeRotation.Write(destination->GetChild("Freeze Rotation", true));
	}

	void Rigidbody::SetAngularVelocity(const Vector3 &velocity)
	{
		m_body->setAngularVelocity(IShape::Convert(velocity));
	}

	void Rigidbody::SetLinearVelocity(const Vector3 &velocity)
	{
		m_body->setLinearVelocity(IShape::Convert(velocity));
	}

	void Rigidbody::AddForce(const Vector3 &force, const Vector3 &position)
	{
		if (m_body == nullptr)
		{
			return;
		}

		m_body->applyForce(IShape::Convert(force), IShape::Convert(position));
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

		auto shape = GetGameObject()->GetComponent<IShape>();

		if (shape != nullptr && isDynamic)
		{
			shape->GetCollisionShape()->calculateLocalInertia(m_mass, localInertia);
		}

		m_body->setMassProps(m_mass, localInertia);
	}

	void Rigidbody::SetStiffness(const float &stiffness)
	{
		m_stiffness = stiffness;
		m_body->setContactStiffnessAndDamping(m_stiffness, m_damping);
	}

	void Rigidbody::SetDamping(const float &damping)
	{
		m_damping = damping;
		m_body->setContactStiffnessAndDamping(m_stiffness, m_damping);
	}

	void Rigidbody::SetFreezePosition(const Constraint3 &freezePosition)
	{
		m_freezePosition = freezePosition;
		m_body->setLinearFactor(IShape::Convert(m_freezePosition, true));
	}

	void Rigidbody::SetFreezeRotation(const Constraint3 &freezeRotation)
	{
		m_freezeRotation = freezeRotation;
		m_body->setAngularFactor(IShape::Convert(m_freezeRotation, true));
	}

	btRigidBody *Rigidbody::CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
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
