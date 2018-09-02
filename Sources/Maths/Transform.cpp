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

	void Transform::Write(LoadedValue &destination)
	{
		m_position.Write(*destination.FindChild("position", true));
		m_rotation.Write(*destination.FindChild("rotation", true));
		m_scaling.Write(*destination.FindChild("scaling", true));
	}

	Transform &Transform::operator=(const Transform &other)
	{
		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scaling = other.m_scaling;
		return *this;
	}

	Transform &Transform::operator=(LoadedValue &value)
	{
		m_position = *value.FindChild("position");
		m_rotation = *value.FindChild("rotation");
		m_scaling = *value.FindChild("scaling");
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
