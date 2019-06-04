#include "Transform.hpp"

namespace acid
{
Transform::Transform(const Vector3f &position, const Vector3f &rotation, const Vector3f &scaling) :
	m_position{position},
	m_rotation{rotation},
	m_scaling{scaling},
	m_dirty{true}
{
}

Transform::Transform(const Vector3f &position, const Vector3f &rotation, const float &scale) :
	m_position{position},
	m_rotation{rotation},
	m_scaling{scale},
	m_dirty{true}
{
}

Transform Transform::Multiply(const Transform &other) const
{
	return Transform(Vector3f(GetWorldMatrix().Transform(Vector4f(other.m_position))), m_rotation + other.m_rotation, m_scaling * other.m_scaling);
}

Matrix4 Transform::GetWorldMatrix() const
{
	if (m_dirty)
	{
		m_worldMatrix = Matrix4::TransformationMatrix(m_position, m_rotation, m_scaling);
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

void Transform::SetScaling(const Vector3f &scaling)
{
	if (m_scaling != scaling)
	{
		m_scaling = scaling;
		m_dirty = true;
	}
}

void Transform::SetDirty(const bool &dirty) const
{
	m_dirty = dirty;
	m_worldMatrix = Matrix4::TransformationMatrix(m_position, m_rotation, m_scaling);
}

std::string Transform::ToString() const
{
	std::stringstream stream;
	stream << "Transform(" << m_position << ", " << m_rotation << ", " << m_scaling << ")";
	return stream.str();
}

bool Transform::operator==(const Transform &other) const
{
	return m_position == other.m_position && m_rotation == other.m_rotation && m_scaling == other.m_scaling;
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
	metadata.GetChild("Position", transform.m_position);
	metadata.GetChild("Rotation", transform.m_rotation);
	metadata.GetChild("Scaling", transform.m_scaling);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Transform &transform)
{
	metadata.SetChild("Position", transform.m_position);
	metadata.SetChild("Rotation", transform.m_rotation);
	metadata.SetChild("Scaling", transform.m_scaling);
	return metadata;
}

std::ostream &operator<<(std::ostream &stream, const Transform &transform)
{
	stream << transform.ToString();
	return stream;
}
}
