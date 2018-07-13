#include "Transform.hpp"

namespace fl
{
	Transform::Transform() :
		m_position(Vector3()),
		m_rotation(Quaternion()),
		m_scaling(Vector3(1.0f, 1.0f, 1.0f))
	{
	}

	Transform::Transform(const Transform &source) :
		m_position(source.m_position),
		m_rotation(source.m_rotation),
		m_scaling(source.m_scaling)
	{
	}

	Transform::Transform(const Vector3 &position, const Quaternion &rotation, const Vector3 &scaling) :
		m_position(position),
		m_rotation(rotation),
		m_scaling(scaling)
	{
	}

	Transform::Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scaling) :
		m_position(position),
		m_rotation(rotation.ToQuaternion()),
		m_scaling(scaling)
	{
	}

	Transform::Transform(const Vector3 &position, const Quaternion &rotation, const float &scale) :
		m_position(position),
		m_rotation(rotation),
		m_scaling(Vector3(scale, scale, scale))
	{
	}

	Transform::Transform(const Vector3 &position, const Vector3 &rotation, const float &scale) :
		m_position(position),
		m_rotation(rotation.ToQuaternion()),
		m_scaling(Vector3(scale, scale, scale))
	{
	}

	Transform::~Transform()
	{
	}

	Matrix4 Transform::GetWorldMatrix() const
	{
		return Matrix4::TransformationMatrix(m_position, m_rotation.ToEuler(), m_scaling);
	}

	Matrix4 Transform::GetModelMatrix() const
	{
		return Matrix4::TransformationMatrix(Vector3::ZERO, m_rotation.ToEuler(), Vector3::ZERO);
	}

	void Transform::Write(LoadedValue *destination)
	{
		m_position.Write(destination->GetChild("position", true));
		m_rotation.Write(destination->GetChild("rotation", true));
		m_scaling.Write(destination->GetChild("scaling", true));
	}

	Transform &Transform::operator=(const Transform &other)
	{
		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scaling = other.m_scaling;
		return *this;
	}

	Transform &Transform::operator=(LoadedValue *value)
	{
		m_position = value->GetChild("position");
		m_rotation = value->GetChild("rotation");
		m_scaling = value->GetChild("scaling");
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
}
