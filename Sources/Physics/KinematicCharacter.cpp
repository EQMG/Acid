#include "KinematicCharacter.hpp"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include "Maths/Transform.hpp"
#include "Scenes/Entity.hpp"
#include "Scenes/Scenes.hpp"

namespace acid {
KinematicCharacter::KinematicCharacter(std::unique_ptr<Collider> &&collider, float mass, float friction) :
	CollisionObject({}, mass, friction),
	up(Vector3f::Up),
	stepHeight(0.0f),
	fallSpeed(55.0f),
	jumpSpeed(10.0f),
	maxHeight(1.5f),
	interpolate(true) {
	AddCollider(std::move(collider));
}

KinematicCharacter::~KinematicCharacter() {
	auto physics = Scenes::Get()->GetPhysics();

	if (physics) {
		// TODO: Are these being deleted?
		physics->GetDynamicsWorld()->removeCollisionObject(ghostObject.get());
		physics->GetDynamicsWorld()->removeAction(controller.get());
	}
}

void KinematicCharacter::Start() {
	if (ghostObject)
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->removeCollisionObject(ghostObject.get());

	if (controller)
		Scenes::Get()->GetPhysics()->GetDynamicsWorld()->removeAction(controller.get());

	CreateShape(true);
	assert((shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE) && "Invalid ghost object shape!");
	gravity = Scenes::Get()->GetPhysics()->GetGravity();
	btVector3 localInertia;

	// Rigidbody is dynamic if and only if mass is non zero, otherwise static.
	if (mass != 0.0f)
		shape->calculateLocalInertia(mass, localInertia);

	auto worldTransform = Collider::Convert(*GetEntity()->GetComponent<Transform>());

	ghostObject = std::make_unique<btPairCachingGhostObject>();
	ghostObject->setWorldTransform(worldTransform);
	Scenes::Get()->GetPhysics()->GetBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	ghostObject->setCollisionShape(shape.get());
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	ghostObject->setFriction(friction);
	ghostObject->setRollingFriction(frictionRolling);
	ghostObject->setSpinningFriction(frictionSpinning);
	ghostObject->setUserPointer(dynamic_cast<CollisionObject *>(this));
	Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addCollisionObject(ghostObject.get(), btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
	body = ghostObject.get();

	controller = std::make_unique<btKinematicCharacterController>(ghostObject.get(), static_cast<btConvexShape *>(shape.get()), 0.03f);
	controller->setGravity(Collider::Convert(gravity));
	controller->setUp(Collider::Convert(up));
	controller->setStepHeight(stepHeight);
	controller->setFallSpeed(fallSpeed);
	controller->setJumpSpeed(jumpSpeed);
	controller->setMaxJumpHeight(maxHeight);
	controller->setUpInterpolate(interpolate);
	Scenes::Get()->GetPhysics()->GetDynamicsWorld()->addAction(controller.get());
}

void KinematicCharacter::Update() {
	if (shape.get() != body->getCollisionShape())
		body->setCollisionShape(shape.get());

	auto &transform = *GetEntity()->GetComponent<Transform>();
	auto worldTransform = ghostObject->getWorldTransform();
	transform = Collider::Convert(worldTransform, transform.GetScale());

	linearVelocity = Collider::Convert(controller->getLinearVelocity());
	angularVelocity = Collider::Convert(controller->getAngularVelocity());
}

bool KinematicCharacter::InFrustum(const Frustum &frustum) {
	btVector3 min;
	btVector3 max;

	if (body && shape)
		shape->getAabb(Collider::Convert(*GetEntity()->GetComponent<Transform>()), min, max);

	return frustum.CubeInFrustum(Collider::Convert(min), Collider::Convert(max));
}

void KinematicCharacter::ClearForces() {
	//controller->clearForces();
}

void KinematicCharacter::SetMass(float mass) {
	this->mass = mass;
	RecalculateMass();
}

void KinematicCharacter::SetGravity(const Vector3f &gravity) {
	this->gravity = gravity;
	controller->setGravity(Collider::Convert(gravity));
}

void KinematicCharacter::SetLinearFactor(const Vector3f &linearFactor) {
	this->linearFactor = linearFactor;
	//controller->setLinearFactor(Collider::Convert(linearFactor)); // TODO
}

void KinematicCharacter::SetAngularFactor(const Vector3f &angularFactor) {
	this->angularFactor = angularFactor;
	//controller->setAngularFactor(Collider::Convert(angularFactor)); // TODO
}

void KinematicCharacter::SetLinearVelocity(const Vector3f &linearVelocity) {
	this->linearVelocity = linearVelocity;
	controller->setLinearVelocity(Collider::Convert(linearVelocity));
}

void KinematicCharacter::SetAngularVelocity(const Vector3f &angularVelocity) {
	this->angularVelocity = angularVelocity;
	controller->setAngularVelocity(Collider::Convert(angularVelocity));
}

void KinematicCharacter::SetUp(const Vector3f &up) {
	this->up = up;
	controller->setUp(Collider::Convert(up));
}

void KinematicCharacter::SetStepHeight(float stepHeight) {
	this->stepHeight = stepHeight;
	controller->setStepHeight(stepHeight);
}

void KinematicCharacter::SetFallSpeed(float fallSpeed) {
	this->fallSpeed = fallSpeed;
	controller->setFallSpeed(fallSpeed);
}

void KinematicCharacter::SetJumpSpeed(float jumpSpeed) {
	this->jumpSpeed = jumpSpeed;
	controller->setJumpSpeed(jumpSpeed);
}

void KinematicCharacter::SetMaxJumpHeight(float maxHeight) {
	this->maxHeight = maxHeight;
	controller->setMaxJumpHeight(maxHeight);
}

void KinematicCharacter::SetInterpolate(bool interpolate) {
	this->interpolate = interpolate;
	controller->setUpInterpolate(interpolate);
}

bool KinematicCharacter::IsOnGround() const {
	return controller->onGround();
}

void KinematicCharacter::Jump(const Vector3f &direction) {
	controller->jump(Collider::Convert(direction));
}

void KinematicCharacter::SetWalkDirection(const Vector3f &direction) {
	controller->setWalkDirection(Collider::Convert(direction));
}

const Node &operator>>(const Node &node, KinematicCharacter &character) {
	node["colliders"].Get(character.colliders);
	node["mass"].Get(character.mass);
	node["friction"].Get(character.friction);
	node["frictionRolling"].Get(character.frictionRolling);
	node["frictionSpinning"].Get(character.frictionSpinning);
	node["up"].Get(character.up);
	node["stepHeight"].Get(character.stepHeight);
	node["fallSpeed"].Get(character.fallSpeed);
	node["jumpSpeed"].Get(character.jumpSpeed);
	node["maxHeight"].Get(character.maxHeight);
	node["interpolate"].Get(character.interpolate);
	return node;
}

Node &operator<<(Node &node, const KinematicCharacter &character) {
	node["colliders"].Set(character.colliders);
	node["mass"].Set(character.mass);
	node["friction"].Set(character.friction);
	node["frictionRolling"].Set(character.frictionRolling);
	node["frictionSpinning"].Set(character.frictionSpinning);
	node["up"].Set(character.up);
	node["stepHeight"].Set(character.stepHeight);
	node["fallSpeed"].Set(character.fallSpeed);
	node["jumpSpeed"].Set(character.jumpSpeed);
	node["maxHeight"].Set(character.maxHeight);
	node["interpolate"].Set(character.interpolate);
	return node;
}

void KinematicCharacter::RecalculateMass() {
	// TODO
}
}
