#include "Transform.hpp"

namespace Flounder
{
	Transform::Transform() :
		m_position(new Vector3()),
		m_rotation(new Vector3()),
		m_scaling(new Vector3(1.0f, 1.0f, 1.0f))
	{
	}

	Transform::Transform(const Transform &source) :
		m_position(new Vector3(*source.m_position)),
		m_rotation(new Vector3(*source.m_rotation)),
		m_scaling(new Vector3(*source.m_scaling))
	{
	}

	Transform::Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scaling) :
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_scaling(new Vector3(scaling))
	{
	}

	Transform::Transform(const Vector3 &position, const Vector3 &rotation, const float &scale) :
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_scaling(new Vector3(scale, scale, scale))
	{
	}

	Transform::Transform(LoadedValue *value)
	{
		Set(value);
	}

	Transform::~Transform()
	{
		delete m_position;
		delete m_rotation;
		delete m_scaling;
	}

	Matrix4 *Transform::GetWorldMatrix(Matrix4 *destination) const
	{
		return Matrix4::TransformationMatrix(*m_position, *m_rotation, *m_scaling, destination);
	}

	Matrix4 *Transform::GetModelMatrix(Matrix4 *destination) const
	{
		return Matrix4::TransformationMatrix(Vector3(), *m_rotation, Vector3(), destination);
	}

	Transform *Transform::Set(const Transform &source)
	{
		*m_position = *source.m_position;
		*m_rotation = *source.m_rotation;
		*m_scaling = *source.m_scaling;
		return this;
	}

	Transform *Transform::Set(LoadedValue *value)
	{
		m_position->Set(value->GetChild("position"));
		m_rotation->Set(value->GetChild("rotation"));
		m_scaling->Set(value->GetChild("scaling"));
		return this;
	}

	void Transform::Write(LoadedValue *destination)
	{
		m_position->Write(destination->GetChild("position", true));
		m_rotation->Write(destination->GetChild("rotation", true));
		m_scaling->Write(destination->GetChild("scaling", true));
	}

	Transform &Transform::operator=(const Transform &other)
	{
		return *Set(other);
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
