#include "CollisionObject.hpp"

#include <cassert>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <LinearMath/btDefaultMotionState.h>
#include "Scenes/Entity.hpp"
#include "Scenes/Scenes.hpp"
#include "Colliders/Collider.hpp"

namespace acid
{
	CollisionObject::CollisionObject(const float &friction) :
		m_friction(friction),
		m_frictionRolling(0.1f),
		m_frictionSpinning(0.2f),
		m_shape(nullptr),
		m_body(nullptr),
		m_forces(std::vector<std::unique_ptr<Force>>()),
		m_collisionEvents(Observer<CollisionObject *>()),
		m_separationEvents(Observer<CollisionObject *>())
	{
	}

	CollisionObject::~CollisionObject()
	{
	}

	Force *CollisionObject::AddForce(Force *force)
	{
		m_forces.emplace_back(force);
		return force;
	}

	void CollisionObject::SetChildTransform(Collider *child, const Transform &transform)
	{
		auto compoundShape = dynamic_cast<btCompoundShape *>(m_shape.get());

		if (compoundShape == nullptr)
		{
			return;
		}

		for (int i = 0; i < compoundShape->getNumChildShapes(); i++)
		{
			if (compoundShape->getChildShape(i) == child->GetCollisionShape())
			{
				compoundShape->updateChildTransform(i, Collider::Convert(transform));
				break;
			}
		}

		RecalculateMass();
	}

	void CollisionObject::AddChild(Collider *child)
	{
		auto compoundShape = dynamic_cast<btCompoundShape *>(m_shape.get());

		if (compoundShape == nullptr)
		{
			return;
		}

		compoundShape->addChildShape(Collider::Convert(child->GetLocalTransform()), child->GetCollisionShape());
		RecalculateMass();
	}

	void CollisionObject::RemoveChild(Collider *child)
	{
		auto compoundShape = dynamic_cast<btCompoundShape *>(m_shape.get());

		if (compoundShape == nullptr)
		{
			return;
		}

		compoundShape->removeChildShape(child->GetCollisionShape());
		RecalculateMass();
	}

	void CollisionObject::SetIgnoreCollisionCheck(CollisionObject *other, const bool &ignore)
	{
		m_body->setIgnoreCollisionCheck(other->m_body.get(), ignore);
	}

	void CollisionObject::SetFriction(const float &friction)
	{
		m_friction = friction;
		m_body->setFriction(m_friction);
	}

	void CollisionObject::SetFrictionRolling(const float &frictionRolling)
	{
		m_frictionRolling = frictionRolling;
		m_body->setRollingFriction(m_frictionRolling);
	}

	void CollisionObject::SetFrictionSpinning(const float &frictionSpinning)
	{
		m_frictionSpinning = frictionSpinning;
		m_body->setSpinningFriction(m_frictionSpinning);
	}

	void CollisionObject::CreateShape(const bool &forceSingle)
	{
		auto colliders = GetParent()->GetComponents<Collider>();

		if (forceSingle) // && colliders.size() == 1
		{
			m_shape.reset(colliders[0]->GetCollisionShape());
			return;
		}
		else if (colliders.empty())
		{
			m_shape = nullptr;
			return;
		}

		if (dynamic_cast<btCompoundShape *>(m_shape.get()) == nullptr)
		{
			m_shape.release();
		}

		m_shape = std::make_unique<btCompoundShape>();
		auto compoundShape = dynamic_cast<btCompoundShape *>(m_shape.get());

		for (int i = 0; i < compoundShape->getNumChildShapes(); i++)
		{
			compoundShape->removeChildShapeByIndex(i);
		}

		for (auto &collider : colliders)
		{
			compoundShape->addChildShape(Collider::Convert(collider->GetLocalTransform()), collider->GetCollisionShape());
		}

		RecalculateMass();
	}
}
