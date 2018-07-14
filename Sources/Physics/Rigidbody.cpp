#include "Rigidbody.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "Models/Model.hpp"
#include "Scenes/Scenes.hpp"

namespace fl
{
	Rigidbody::Rigidbody(const float &mass, const float &drag, const bool &useGravity, std::shared_ptr<IShape> shape, const Constraint3 &freezePosition, const Constraint3 &freezeRotation) :
		IComponent(),
		m_mass(mass),
		m_drag(drag),
		m_useGravity(useGravity),
		m_freezePosition(freezePosition),
		m_freezeRotation(freezeRotation),
		m_shape(shape),
		m_worldTransform(btTransform()),
		m_body(nullptr)
	{
		m_worldTransform.setIdentity();
		m_worldTransform.setOrigin(btVector3());

		if (m_shape != nullptr)
		{
			m_body = Scenes::Get()->CreateRigidBody(m_mass, m_worldTransform, m_shape->GetCollisionShape());
			m_body->setContactStiffnessAndDamping(300.0f, 10.0f);
			Scenes::Get()->GetDynamicsWorld()->addRigidBody(m_body);
		}
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
		m_worldTransform.setOrigin(btVector3(position.m_x, position.m_y, position.m_z));
		m_worldTransform.setRotation(btQuaternion(rotation.m_x, rotation.m_y, rotation.m_z, rotation.m_w));

		if (m_body != nullptr)
		{
			m_body->setWorldTransform(m_worldTransform);
		}
	}

	void Rigidbody::Update()
	{
		if (m_body == nullptr)
		{
			return;
		}

		auto &transform = GetGameObject()->GetTransform();

		if (m_freezePosition != Constraint3::ONE)
		{
			btVector3 position = m_body->getWorldTransform().getOrigin();
			transform.SetPosition(Vector3(position.getX(), position.getY(), position.getZ()));
		}

		if (m_freezeRotation != Constraint3::ONE)
		{
			btQuaternion rotation = m_body->getWorldTransform().getRotation();
			transform.SetRotation(Quaternion(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW()));
		}

	//	m_worldTransform.setIdentity();
	//	m_worldTransform.setOrigin(btVector3(transform.GetPosition().m_x, transform.GetPosition().m_y, transform.GetPosition().m_z));
	//	m_worldTransform.setRotation(btQuaternion(transform.GetRotation().m_x, transform.GetRotation().m_y, transform.GetRotation().m_z, transform.GetRotation().m_w));

	//	m_body->setWorldTransform(m_worldTransform);
	//	m_body->setLinearVelocity(m_velocity);
	}

	void Rigidbody::Load(LoadedValue *value)
	{
		m_mass = value->GetChild("Mass")->Get<float>();
		m_drag = value->GetChild("Drag")->Get<float>();
		m_useGravity = value->GetChild("Use Gravity")->Get<bool>();
		m_freezePosition = value->GetChild("Freeze Position");
		m_freezeRotation = value->GetChild("Freeze Rotation");
	}

	void Rigidbody::Write(LoadedValue *destination)
	{
		destination->GetChild("Mass", true)->Set(m_mass);
		destination->GetChild("Drag", true)->Set(m_drag);
		destination->GetChild("Use Gravity", true)->Set(m_useGravity);
		m_freezePosition.Write(destination->GetChild("Freeze Position", true));
		m_freezeRotation.Write(destination->GetChild("Freeze Rotation", true));
	}
}
