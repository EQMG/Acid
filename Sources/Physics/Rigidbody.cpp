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
#include "Physics.hpp"

namespace acid {
const bool Rigidbody::Registered = Register("rigidbody");

Rigidbody::Rigidbody(std::unique_ptr<Collider> &&collider, float mass, float friction, const Vector3f &linearFactor, const Vector3f &angularFactor) :
	CollisionObject({}, mass, friction, linearFactor, angularFactor) {
	AddCollider(std::move(collider));
}

Rigidbody::~Rigidbody() {
	if (auto body = btRigidBody::upcast(this->body); body && body->getMotionState())
		delete body->getMotionState();

	if (auto physics = Scenes::Get()->GetScene()->GetSystem<Physics>())
		physics->GetDynamicsWorld()->removeRigidBody(rigidBody.get());
}

void Rigidbody::Start() {
	auto physics = Scenes::Get()->GetScene()->GetSystem<Physics>();
	
	if (rigidBody)
		physics->GetDynamicsWorld()->removeRigidBody(rigidBody.get());

	CreateShape();
	assert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE) && "Invalid rigidbody shape!");
	gravity = physics->GetGravity();
	btVector3 localInertia;

	// Rigidbody is dynamic if and only if mass is non zero, otherwise static.
	if (mass != 0.0f)
		shape->calculateLocalInertia(mass, localInertia);

	auto worldTransform = Collider::Convert(*GetEntity()->GetComponent<Transform>());

	// Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects.
	auto motionState = new btDefaultMotionState(worldTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, motionState, shape.get(), localInertia);

	rigidBody = std::make_unique<btRigidBody>(cInfo);
	//rigidBody->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
	rigidBody->setWorldTransform(worldTransform);
	//rigidBody->setContactStiffnessAndDamping(1000.0f, 0.1f);
	rigidBody->setFriction(friction);
	rigidBody->setRollingFriction(frictionRolling);
	rigidBody->setSpinningFriction(frictionSpinning);
	rigidBody->setGravity(Collider::Convert(gravity));
	rigidBody->setLinearFactor(Collider::Convert(linearFactor));
	rigidBody->setAngularFactor(Collider::Convert(angularFactor));
	rigidBody->setUserPointer(dynamic_cast<CollisionObject *>(this));
	body = rigidBody.get();
	physics->GetDynamicsWorld()->addRigidBody(rigidBody.get());
	rigidBody->activate(true);
	RecalculateMass();
}

void Rigidbody::Update() {
	if (shape.get() != body->getCollisionShape())
		body->setCollisionShape(shape.get());

	auto delta = Engine::Get()->GetDelta();

	for (auto it = forces.begin(); it != forces.end();) {
		(*it)->Update(delta);
		rigidBody->applyForce(Collider::Convert((*it)->GetForce()), Collider::Convert((*it)->GetPosition()));

		if ((*it)->IsExpired()) {
			it = forces.erase(it);
			continue;
		}

		++it;
	}

	auto &transform = *GetEntity()->GetComponent<Transform>();
	btTransform motionTransform;
	rigidBody->getMotionState()->getWorldTransform(motionTransform);
	transform = Collider::Convert(motionTransform, transform.GetScale());

	shape->setLocalScaling(Collider::Convert(transform.GetScale()));
	//rigidBody->getMotionState()->setWorldTransform(Collider::Convert(transform));
	linearVelocity = Collider::Convert(rigidBody->getLinearVelocity());
	angularVelocity = Collider::Convert(rigidBody->getAngularVelocity());
}

bool Rigidbody::InFrustum(const Frustum &frustum) {
	btVector3 min;
	btVector3 max;

	if (body && shape)
		rigidBody->getAabb(min, max);

	return frustum.CubeInFrustum(Collider::Convert(min), Collider::Convert(max));
}

void Rigidbody::ClearForces() {
	if (rigidBody)
		rigidBody->clearForces();
}

void Rigidbody::SetMass(float mass) {
	this->mass = mass;
	RecalculateMass();
}

void Rigidbody::SetGravity(const Vector3f &gravity) {
	this->gravity = gravity;

	if (rigidBody)
		rigidBody->setGravity(Collider::Convert(gravity));
}

void Rigidbody::SetLinearFactor(const Vector3f &linearFactor) {
	this->linearFactor = linearFactor;

	if (rigidBody)
		rigidBody->setLinearFactor(Collider::Convert(linearFactor));
}

void Rigidbody::SetAngularFactor(const Vector3f &angularFactor) {
	this->angularFactor = angularFactor;

	if (rigidBody)
		rigidBody->setAngularFactor(Collider::Convert(angularFactor));
}

void Rigidbody::SetLinearVelocity(const Vector3f &linearVelocity) {
	this->linearVelocity = linearVelocity;

	if (rigidBody)
		rigidBody->setLinearVelocity(Collider::Convert(linearVelocity));
}

void Rigidbody::SetAngularVelocity(const Vector3f &angularVelocity) {
	this->angularVelocity = angularVelocity;

	if (rigidBody)
		rigidBody->setAngularVelocity(Collider::Convert(angularVelocity));
}

const Node &operator>>(const Node &node, Rigidbody &rigidbody) {
	node["colliders"].Get(rigidbody.colliders);
	node["mass"].Get(rigidbody.mass);
	node["friction"].Get(rigidbody.friction);
	node["frictionRolling"].Get(rigidbody.frictionRolling);
	node["frictionSpinning"].Get(rigidbody.frictionSpinning);
	node["linearFactor"].Get(rigidbody.linearFactor);
	node["angularFactor"].Get(rigidbody.angularFactor);
	return node;
}

Node &operator<<(Node &node, const Rigidbody &rigidbody) {
	node["colliders"].Set(rigidbody.colliders);
	node["mass"].Set(rigidbody.mass);
	node["friction"].Set(rigidbody.friction);
	node["frictionRolling"].Set(rigidbody.frictionRolling);
	node["frictionSpinning"].Set(rigidbody.frictionSpinning);
	node["linearFactor"].Set(rigidbody.linearFactor);
	node["angularFactor"].Set(rigidbody.angularFactor);
	return node;
}

void Rigidbody::RecalculateMass() {
	if (!rigidBody) return;

	auto isDynamic = mass != 0.0f;

	btVector3 localInertia;

	if (!colliders.empty() && isDynamic)
		colliders[0]->GetCollisionShape()->calculateLocalInertia(mass, localInertia);

	rigidBody->setMassProps(mass, localInertia);
}
}
