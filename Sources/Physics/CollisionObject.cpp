#include "CollisionObject.hpp"

#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include "Colliders/Collider.hpp"

namespace acid {
CollisionObject::CollisionObject(std::vector<std::unique_ptr<Collider>> &&colliders, float mass, float friction, const Vector3f &linearFactor, const Vector3f &angularFactor) :
	colliders(std::move(colliders)),
	mass(mass),
	friction(friction),
	frictionRolling(0.1f),
	frictionSpinning(0.2f),
	linearFactor(linearFactor),
	angularFactor(angularFactor) {
	for (auto &collider : colliders) {
		collider->OnTransformChange().Add([this](Collider *collider, const Transform &localTransform) {
			SetChildTransform(collider, localTransform);
		}, this);
	}
}

CollisionObject::~CollisionObject() {
}

Collider *CollisionObject::AddCollider(std::unique_ptr<Collider> &&collider) {
	if (!collider) return nullptr;
	auto ret = colliders.emplace_back(std::move(collider)).get();
	ret->OnTransformChange().Add([this](Collider *collider, const Transform &localTransform) {
		SetChildTransform(collider, localTransform);
	}, this);
	return ret;
}

void CollisionObject::RemoveCollider(Collider *collider) {
	if (!collider) return;
	colliders.erase(std::remove_if(colliders.begin(), colliders.end(), [collider](std::unique_ptr<Collider> &c) {
		return c.get() == collider;
	}), colliders.end());
}

Force *CollisionObject::AddForce(std::unique_ptr<Force> &&force) {
	return forces.emplace_back(std::move(force)).get();
}

void CollisionObject::SetChildTransform(Collider *child, const Transform &transform) {
	auto compoundShape = dynamic_cast<btCompoundShape *>(shape.get());

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
	if (auto compoundShape = dynamic_cast<btCompoundShape *>(shape.get())) {
		compoundShape->addChildShape(Collider::Convert(child->GetLocalTransform()), child->GetCollisionShape());
		RecalculateMass();
	}
}

void CollisionObject::RemoveChild(Collider *child) {
	if (auto compoundShape = dynamic_cast<btCompoundShape *>(shape.get())) {
		compoundShape->removeChildShape(child->GetCollisionShape());
		RecalculateMass();
	}
}

void CollisionObject::SetIgnoreCollisionCheck(CollisionObject *other, bool ignore) {
	body->setIgnoreCollisionCheck(other->body, ignore);
}

void CollisionObject::SetFriction(float friction) {
	this->friction = friction;
	body->setFriction(friction);
}

void CollisionObject::SetFrictionRolling(float frictionRolling) {
	this->frictionRolling = frictionRolling;
	body->setRollingFriction(frictionRolling);
}

void CollisionObject::SetFrictionSpinning(float frictionSpinning) {
	this->frictionSpinning = frictionSpinning;
	body->setSpinningFriction(frictionSpinning);
}

void CollisionObject::CreateShape(bool forceSingle) {
	if (forceSingle) { // && colliders.size() == 1
		shape.reset(colliders[0]->GetCollisionShape());
		return;
	}
	if (colliders.empty()) {
		shape = nullptr;
		return;
	}

	if (!dynamic_cast<btCompoundShape *>(shape.get())) {
		shape.release();
	}

	auto compoundShape = new btCompoundShape();

	for (int32_t i = 0; i < compoundShape->getNumChildShapes(); i++) {
		compoundShape->removeChildShapeByIndex(i);
	}

	for (const auto &collider : colliders) {
		compoundShape->addChildShape(Collider::Convert(collider->GetLocalTransform()), collider->GetCollisionShape());
	}

	shape.reset(compoundShape);
	RecalculateMass();
}
}
