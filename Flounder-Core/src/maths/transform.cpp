#include "Transform.hpp"

namespace flounder
{
	Transform::Transform() :
		m_position(new vector3()),
		m_rotation(new vector3()),
		m_scaling(new vector3())
	{
	}

	Transform::Transform(const Transform &source) :
		m_position(new vector3(*source.m_position)),
		m_rotation(new vector3(*source.m_rotation)),
		m_scaling(new vector3(*source.m_scaling))
	{
	}

	Transform::Transform(const vector3 &position, const vector3 &rotation, const vector3 &scaling) :
		m_position(new vector3(position)),
		m_rotation(new vector3(rotation)),
		m_scaling(new vector3(scaling))
	{
	}

	Transform::~Transform()
	{
		delete m_position;
		delete m_rotation;
		delete m_scaling;
	}

	matrix4x4 *Transform::GetWorldMatrix(matrix4x4 *destination) const
	{
		return matrix4x4::transformationMatrix(*m_position, *m_rotation, *m_scaling, destination);
	}

	matrix4x4 *Transform::GetModelMatrix(matrix4x4 *destination) const
	{
		return matrix4x4::transformationMatrix(vector3(), *m_rotation, vector3(), destination);
	}

	void Transform::Set(const Transform &source) const
	{
		m_position->set(*source.m_position);
		m_rotation->set(*source.m_rotation);
		m_scaling->set(*source.m_scaling);
	}
}
