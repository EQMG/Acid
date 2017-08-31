#include "Transform.hpp"

namespace Flounder
{
	Transform::Transform() :
		m_position(new Vector3()),
		m_rotation(new Vector3()),
		m_scaling(new Vector3())
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

	Transform::~Transform()
	{
		delete m_position;
		delete m_rotation;
		delete m_scaling;
	}

	Matrix4 *Transform::GetWorldMatrix(Matrix4 *destination) const
	{
		return Matrix4::transformationMatrix(*m_position, *m_rotation, *m_scaling, destination);
	}

	Matrix4 *Transform::GetModelMatrix(Matrix4 *destination) const
	{
		return Matrix4::transformationMatrix(Vector3(), *m_rotation, Vector3(), destination);
	}

	void Transform::Set(const Transform &source) const
	{
		m_position->set(*source.m_position);
		m_rotation->set(*source.m_rotation);
		m_scaling->set(*source.m_scaling);
	}
}
