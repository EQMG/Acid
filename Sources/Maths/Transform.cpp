#include "Transform.hpp"

#include "Scenes/Entity.hpp"

namespace acid {
Transform::Transform(const Vector3f &position, const Vector3f &rotation, const Vector3f &scale) :
	position(position),
	rotation(rotation),
	scale(scale) {
}

Transform::~Transform() {
	delete worldTransform;

	if (parent) {
		parent->RemoveChild(this);
	}

	for (auto &child : children) {
		child->parent = nullptr;
	}
}

Transform Transform::Multiply(const Transform &other) const {
	return {Vector3f(GetWorldMatrix().Transform(Vector4f(other.position))), rotation + other.rotation, scale * other.scale};
}

Matrix4 Transform::GetWorldMatrix() const {
	auto worldTransform = GetWorldTransform();
	return Matrix4::TransformationMatrix(worldTransform->position, worldTransform->rotation, worldTransform->scale);
}

Vector3f Transform::GetPosition() const {
	return GetWorldTransform()->position;
}

Vector3f Transform::GetRotation() const {
	return GetWorldTransform()->rotation;
}

Vector3f Transform::GetScale() const {
	return GetWorldTransform()->scale;
}

void Transform::SetParent(Transform *parent) {
	if (parent) {
		parent->RemoveChild(this);
	}

	this->parent = parent;

	if (parent) {
		parent->AddChild(this);
	}
}

void Transform::SetParent(Entity *parent) {
	SetParent(parent->GetComponent<Transform>());
}

bool Transform::operator==(const Transform &other) const {
	return position == other.position && rotation == other.rotation && scale == other.scale;
}

bool Transform::operator!=(const Transform &other) const {
	return !operator==(other);
}

Transform operator*(const Transform &left, const Transform &right) {
	return left.Multiply(right);
}

Transform &Transform::operator*=(const Transform &other) {
	return *this = Multiply(other);
}

const Node &operator>>(const Node &node, Transform &transform) {
	node["position"].Get(transform.position);
	node["rotation"].Get(transform.rotation);
	node["scale"].Get(transform.scale);
	return node;
}

Node &operator<<(Node &node, const Transform &transform) {
	node["position"].Set(transform.position);
	node["rotation"].Set(transform.rotation);
	node["scale"].Set(transform.scale);
	return node;
}

std::ostream &operator<<(std::ostream &stream, const Transform &transform) {
	return stream << transform.position << ", " << transform.rotation << ", " << transform.scale;
}

const Transform *Transform::GetWorldTransform() const {
	if (!parent) {
		if (worldTransform) {
			delete worldTransform;
			worldTransform = nullptr;
		}

		return this;
	}

	if (!worldTransform) {
		worldTransform = new Transform();
	}

	*worldTransform = *parent->GetWorldTransform() * *this;
	return worldTransform;
}

void Transform::AddChild(Transform *child) {
	children.emplace_back(child);
}

void Transform::RemoveChild(Transform *child) {
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}
}
