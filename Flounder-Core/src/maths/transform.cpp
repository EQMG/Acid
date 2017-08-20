#include "transform.h"

namespace flounder
{
	transform::transform()
	{
		m_position = new vector3();
		m_rotation = new vector3();
		m_scaling = new vector3();
	}

	transform::transform(const vector3 &position)
	{
		m_position = new vector3(position);
		m_rotation = new vector3();
		m_scaling = new vector3();
	}

	transform::transform(const vector3 &position, const vector3 &rotation)
	{
		m_position = new vector3(position);
		m_rotation = new vector3(rotation);
		m_scaling = new vector3();
	}

	transform::transform(const vector3 &position, const vector3 &rotation, const vector3 &scaling)
	{
		m_position = new vector3(position);
		m_rotation = new vector3(rotation);
		m_scaling = new vector3(scaling);
	}

	transform::transform(const vector3 & position, const vector3 & rotation, const float &scale)
	{
		m_position = new vector3(position);
		m_rotation = new vector3(rotation);
		m_scaling = new vector3(scale, scale, scale);
	}

	transform::~transform()
	{
		delete m_position;
		delete m_rotation;
		delete m_scaling;
	}

	matrix4x4 *transform::worldMatrix(matrix4x4 *destination) const
	{
		return matrix4x4::transformationMatrix(*m_position, *m_rotation, *m_scaling, destination);
	}

	matrix4x4 *transform::modelMatrix(matrix4x4 *destination) const
	{
		return matrix4x4::transformationMatrix(vector3(), *m_rotation, vector3(), destination);
	}
}
