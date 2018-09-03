#include "Transform.hpp"

namespace acid
{
	Transform::Transform() :
		m_position(Vector3()),
		m_rotation(Vector3()),
		m_scaling(Vector3(1.0f, 1.0f, 1.0f))
	{
	}

	Transform::Transform(const Transform &source) :
		m_position(source.m_position),
		m_rotation(source.m_rotation),
		m_scaling(source.m_scaling)
	{
	}

	Transform::Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scaling) :
		m_position(position),
		m_rotation(rotation),
		m_scaling(scaling)
	{
	}

	Transform::Transform(const Vector3 &position, const Vector3 &rotation, const float &scale) :
		m_position(position),
		m_rotation(rotation),
		m_scaling(Vector3(scale, scale, scale))
	{
	}

	Transform::~Transform()
	{
	}

	Matrix4 Transform::GetWorldMatrix() const
	{
		return Matrix4::TransformationMatrix(m_position, m_rotation, m_scaling);
	}

	Matrix4 Transform::GetModelMatrix() const
	{
		return Matrix4::TransformationMatrix(Vector3::ZERO, m_rotation, Vector3::ZERO);
	}

	void Transform::Decode(const Serialized &serialized)
	{
		m_position = serialized.GetChild<Vector3>("Position");
		m_rotation = serialized.GetChild<Vector3>("Rotation");
		m_scaling = serialized.GetChild<Vector3>("Scaling");
	}

	void Transform::Encode(Serialized &serialized) const
	{
		serialized.SetChild<Vector3>("Position", m_position);
		serialized.SetChild<Vector3>("Rotation", m_rotation);
		serialized.SetChild<Vector3>("Scaling", m_scaling);
	}

	Transform &Transform::operator=(const Transform &other)
	{
		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scaling = other.m_scaling;
		return *this;
	}

	bool Transform::operator==(const Transform &other) const
	{
		return m_position == other.m_position && m_rotation == other.m_rotation && m_scaling == other.m_scaling;
	}

	bool Transform::operator!=(const Transform &other) const
	{
		return !(*this == other);
	}

	std::ostream &operator<<(std::ostream &stream, const Transform &transform)
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
