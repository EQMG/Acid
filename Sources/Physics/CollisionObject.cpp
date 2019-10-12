#include "CollisionObject.hpp"

#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include "Colliders/Collider.hpp"

namespace acid {
CollisionObject::CollisionObject(std::vector<std::unique_ptr<Collider>> &&colliders, float mass, float friction, const Vector3f &linearFactor, const Vector3f &angularFactor) :
	m_colliders(std::move(colliders)),
	m_mass(mass),
	m_friction(friction),
	m_frictionRolling(0.1f),
	m_frictionSpinning(0.2f),
	m_linearFactor(linearFactor),
	m_angularFactor(angularFactor) {
	for (auto &collider : m_colliders) {
		collider->OnTransformChange().Add([this](Collider *collider, const Transform &localTransform) {
			SetChildTransform(collider, localTransform);
		}, this);
	}
}

CollisionObject::~CollisionObject() {
}

Collider *CollisionObject::AddCollider(std::unique_ptr<Collider> &&collider) {
	if (!collider) return nullptr;
	auto ret = m_colliders.emplace_back(std::move(collider)).get();
	ret->OnTransformChange().Add([this](Collider *collider, const Transform &localTransform) {
		SetChildTransform(collider, localTransform);
	}, this);
	return ret;
}

void CollisionObject::RemoveCollider(Collider *collider) {
	if (!collider) return;
	m_colliders.erase(std::remove_if(m_colliders.begin(), m_colliders.end(), [collider](std::unique_ptr<Collider> &c) {
		return c.get() == collider;
	}), m_colliders.end());
}

Force *CollisionObject::AddForce(std::unique_ptr<Force> &&force) {
	return m_forces.emplace_back(std::move(force)).get();
}

void CollisionObject::SetChildTransform(Collider *child, const Transform &transform) {
	auto compoundShape = dynamic_cast<btCompoundShape *>(m_shape.get());

	if (!compoundShape) {
		return;
	}

	for (int32_t i = 0; i < compoundShape->getNumChildShapes(); i++) {
		if (compoundShape->getChildShape(i) == child->GetCollisionShape()) {
			compoundShape->updateChildTransform(i, Collider::Convert(transform));
			break;
		}
	}

	RecalculateMass();
}

void CollisionObject::AddChild(Collider *child) {
	if (auto compoundShape = dynamic_cast<btCompoundShape *>(m_shape.get())) {
		compoundShape->addChildShape(Collider::Convert(child->GetLocalTransform()), child->GetCollisionShape());
		RecalculateMass();
	}
}

void CollisionObject::RemoveChild(Collider *child) {
	if (auto compoundShape = dynamic_cast<btCompoundShape *>(m_shape.get())) {
		compoundShape->removeChildShape(child->GetCollisionShape());
		RecalculateMass();
	}
}

void CollisionObject::SetIgnoreCollisionCheck(CollisionObject *other, bool ignore) {
	m_body->setIgnoreCollisionCheck(other->m_body, ignore);
}

void CollisionObject::SetFriction(float friction) {
	m_friction = friction;
	m_body->setFriction(m_friction);
}

void CollisionObject::SetFrictionRolling(float frictionRolling) {
	m_frictionRolling = frictionRolling;
	m_body->setRollingFriction(m_frictionRolling);
}

void CollisionObject::SetFrictionSpinning(float frictionSpinning) {
	m_frictionSpinning = frictionSpinning;
	m_body->setSpinningFriction(m_frictionSpinning);
}

void CollisionObject::CreateShape(bool forceSingle) {
	if (forceSingle) { // && colliders.size() == 1
		m_shape.reset(m_colliders[0]->GetCollisionShape());
		return;
	}
	if (m_colliders.empty()) {
		m_shape = nullptr;
		return;
	}

	if (!dynamic_cast<btCompoundShape *>(m_shape.get())) {
		m_shape.release();
	}

	auto compoundShape = new btCompoundShape();

	for (int32_t i = 0; i < compoundShape->getNumChildShapes(); i++) {
		compoundShape->removeChildShapeByIndex(i);
	}

	for (const auto &collider : m_colliders) {
		compoundShape->addChildShape(Collider::Convert(collider->GetLocalTransform()), collider->GetCollisionShape());
	}

	m_shape.reset(compoundShape);
	RecalculateMass();
}
}
