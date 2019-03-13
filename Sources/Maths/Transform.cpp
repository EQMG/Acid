#include "Transform.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
const Transform Transform::Identity = Transform(Vector3::Zero, Vector3::Zero, Vector3::One);

Transform::Transform(const Vector3& position, const Vector3& rotation, const Vector3& scaling) : m_position(position), m_rotation(rotation), m_scaling(scaling), m_dirty(true) {}

Transform::Transform(const Vector3& position, const Vector3& rotation, const float& scale) : m_position(position), m_rotation(rotation), m_scaling(scale, scale, scale), m_dirty(true) {}

Transform::Transform(const Transform& source) : m_position(source.m_position), m_rotation(source.m_rotation), m_scaling(source.m_scaling), m_dirty(true) {}

void Transform::Start() {}

void Transform::Update() {}

void Transform::Decode(const Metadata& metadata)
{
	metadata.GetChild("Position", m_position);
	metadata.GetChild("Rotation", m_rotation);
	metadata.GetChild("Scaling", m_scaling);
	m_dirty = true;
}

void Transform::Encode(Metadata& metadata) const
{
	metadata.SetChild("Position", m_position);
	metadata.SetChild("Rotation", m_rotation);
	metadata.SetChild("Scaling", m_scaling);
}

Transform Transform::Multiply(const Transform& other) const
{
	return Transform(GetWorldMatrix().Transform(other.m_position), m_rotation + other.m_rotation, m_scaling * other.m_scaling);
}

Matrix4 Transform::GetWorldMatrix() const
{
	if(m_dirty)
		{
			m_worldMatrix = Matrix4::TransformationMatrix(m_position, m_rotation * Maths::DegToRad, m_scaling);
			m_dirty = false;
		}

	return m_worldMatrix;
}

void Transform::SetPosition(const Vector3& position)
{
	if(m_position != position)
		{
			m_position = position;
			m_dirty = true;
		}
}

void Transform::SetRotation(const Vector3& rotation)
{
	if(m_rotation != rotation)
		{
			m_rotation = rotation;
			m_dirty = true;
		}
}

void Transform::SetScaling(const Vector3& scaling)
{
	if(m_scaling != scaling)
		{
			m_scaling = scaling;
			m_dirty = true;
		}
}

void Transform::SetDirty(const bool& dirty) const
{
	m_dirty = dirty;
	m_worldMatrix = Matrix4::TransformationMatrix(m_position, m_rotation * Maths::DegToRad, m_scaling);
}

bool Transform::operator==(const Transform& other) const
{
	return m_position == other.m_position && m_rotation == other.m_rotation && m_scaling == other.m_scaling;
}

bool Transform::operator!=(const Transform& other) const
{
	return !(*this == other);
}

Transform operator*(const Transform& left, const Transform& right)
{
	return left.Multiply(right);
}

Transform& Transform::operator*=(const Transform& other)
{
	return *this = Multiply(other);
}

std::ostream& operator<<(std::ostream& stream, const Transform& transform)
{
	stream << transform.ToString();
	return stream;
}

std::string Transform::ToString() const
{
	std::stringstream result;
	result << "Transform(" << m_position << ", " << m_rotation << ", " << m_scaling << ")";
	return result.str();
}
}
