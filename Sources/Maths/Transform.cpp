#include "Transform.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
Transform::Transform(const Vector3f &position, const Vector3f &rotation, const Vector3f &scale) :
	m_position{position},
	m_rotation{rotation},
	m_scale{scale}
{
}

Transform::~Transform()
{
	delete m_worldTransform;

	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}

	for (auto &child : m_children)
	{
		child->m_parent = nullptr;
	}
}

Transform Transform::Multiply(const Transform &other) const
{
	return {Vector3f{GetWorldMatrix().Transform(Vector4f{other.m_position})}, m_rotation + other.m_rotation, m_scale * other.m_scale};
}

Matrix4 Transform::GetWorldMatrix() const
{
	auto worldTransform{GetWorldTransform()};
	return Matrix4::TransformationMatrix(worldTransform->m_position, worldTransform->m_rotation, worldTransform->m_scale);
}

Vector3f Transform::GetPosition() const
{
	return GetWorldTransform()->m_position;
}

Vector3f Transform::GetRotation() const
{
	return GetWorldTransform()->m_rotation;
}

Vector3f Transform::GetScale() const
{
	return GetWorldTransform()->m_scale;
}

void Transform::SetLocalPosition(const Vector3f &localPosition)
{
	m_position = localPosition;
}

void Transform::SetLocalRotation(const Vector3f &localRotation)
{
	m_rotation = localRotation;
}

void Transform::SetLocalScale(const Vector3f &localScale)
{
	m_scale = localScale;
}

void Transform::SetParent(Transform *parent)
{
	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}
	
	m_parent = parent;

	if (m_parent != nullptr)
	{
		m_parent->AddChild(this);
	}
}

void Transform::SetParent(Entity *parent)
{
	SetParent(parent->GetComponent<Transform>());
}

bool Transform::operator==(const Transform &other) const
{
	return m_position == other.m_position && m_rotation == other.m_rotation && m_scale == other.m_scale;
}

bool Transform::operator!=(const Transform &other) const
{
	return !(*this == other);
}

Transform operator*(const Transform &left, const Transform &right)
{
	return left.Multiply(right);
}

Transform &Transform::operator*=(const Transform &other)
{
	return *this = Multiply(other);
}

const Metadata &operator>>(const Metadata &metadata, Transform &transform)
{
	metadata.GetChild("position", transform.m_position);
	metadata.GetChild("rotation", transform.m_rotation);
	metadata.GetChild("scale", transform.m_scale);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Transform &transform)
{
	metadata.SetChild("position", transform.m_position);
	metadata.SetChild("rotation", transform.m_rotation);
	metadata.SetChild("scale", transform.m_scale);
	return metadata;
}

std::ostream &operator<<(std::ostream &stream, const Transform &transform)
{
	return stream << transform.m_position << ", " << transform.m_rotation << ", " << transform.m_scale;
}

const Transform *Transform::GetWorldTransform() const
{
	if (m_parent == nullptr)
	{
		if (m_worldTransform != nullptr)
		{
			delete m_worldTransform;
			m_worldTransform = nullptr;
		}

		return this;
	}

	if (m_worldTransform == nullptr)
	{
		m_worldTransform = new Transform();
	}

	*m_worldTransform = *m_parent->GetWorldTransform() * *this;
	return m_worldTransform;
}

void Transform::AddChild(Transform *child)
{
	m_children.emplace_back(child);
}

void Transform::RemoveChild(Transform *child)
{
	m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}
}
