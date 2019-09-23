#include "CollisionObject.hpp"

#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include "Scenes/Entity.hpp"
#include "Scenes/Scenes.hpp"
#include "Colliders/Collider.hpp"

namespace acid {
CollisionObject::CollisionObject(float mass, float friction, const Vector3f &linearFactor, const Vector3f &angularFactor) :
	m_mass(mass),
	m_friction(friction),
	m_frictionRolling(0.1f),
	m_frictionSpinning(0.2f),
	m_linearFactor(linearFactor),
	m_angularFactor(angularFactor) {
}

CollisionObject::~CollisionObject() {
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
	auto compoundShape = dynamic_cast<btCompoundShape *>(m_shape.get());

	if (!compoundShape) {
		return;
	}

	compoundShape->addChildShape(Collider::Convert(child->GetLocalTransform()), child->GetCollisionShape());
	RecalculateMass();
}

void CollisionObject::RemoveChild(Collider *child) {
	auto compoundShape = dynamic_cast<btCompoundShape *>(m_shape.get());

	if (!compoundShape) {
		return;
	}

	compoundShape->removeChildShape(child->GetCollisionShape());
	RecalculateMass();
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

void CollisionObject::CreateShape(const std::vector<Collider *> &colliders, bool forceSingle) {
	if (forceSingle) // && colliders.size() == 1
	{
		m_shape.reset(colliders[0]->GetCollisionShape());
		return;
	}
	if (colliders.empty()) {
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

	for (const auto &collider : colliders) {
		compoundShape->addChildShape(Collider::Convert(collider->GetLocalTransform()), collider->GetCollisionShape());
	}

	m_shape.reset(compoundShape);
	RecalculateMass();
}
}
