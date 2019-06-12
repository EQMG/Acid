#include "Transform.hpp"

namespace acid
{
Transform::Transform(const Vector3f &position, const Vector3f &rotation, const Vector3f &scale) :
	m_position{position},
	m_rotation{rotation},
	m_scale{scale},
	m_dirty{true}
{
}

Transform Transform::Multiply(const Transform &other) const
{
	return {Vector3f{GetWorldMatrix().Transform(Vector4f{other.m_position})}, m_rotation + other.m_rotation, m_scale * other.m_scale};
}

Matrix4 Transform::GetWorldMatrix() const
{
	if (m_dirty)
	{
		m_worldMatrix = Matrix4::TransformationMatrix(m_position, m_rotation, m_scale);
		m_dirty = false;
	}

	return m_worldMatrix;
}

void Transform::SetPosition(const Vector3f &position)
{
	if (m_position != position)
	{
		m_position = position;
		m_dirty = true;
	}
}

void Transform::SetRotation(const Vector3f &rotation)
{
	if (m_rotation != rotation)
	{
		m_rotation = rotation;
		m_dirty = true;
	}
}

void Transform::SetScale(const Vector3f &scale)
{
	if (m_scale != scale)
	{
		m_scale = scale;
		m_dirty = true;
	}
}

void Transform::SetDirty(const bool &dirty) const
{
	m_dirty = dirty;
	m_worldMatrix = Matrix4::TransformationMatrix(m_position, m_rotation, m_scale);
}

std::string Transform::ToString() const
{
	std::stringstream stream;
	stream << "Transform(" << m_position << ", " << m_rotation << ", " << m_scale << ")";
	return stream.str();
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
	stream << transform.ToString();
	return stream;
}
}
