#pragma once

#include "vector3.h"
#include "matrix4x4.h"

namespace flounder
{
	class transform
	{
	private:
		vector3 *m_position;
		vector3 *m_rotation;
		vector3 *m_scaling;
	public:
		transform();

		transform(const transform &source);

		transform(const vector3 &position);

		transform(const vector3 &position, const vector3 &rotation);

		transform(const vector3 &position, const vector3 &rotation, const vector3 &scaling);

		transform(const vector3 &position, const vector3 &rotation, const float &scale);

		~transform();

		matrix4x4 *worldMatrix(matrix4x4 *destination) const;

		matrix4x4 *modelMatrix(matrix4x4 *destination) const;

		void set(const transform &source);

		inline vector3 *getPosition() const { return m_position; }

		inline void setPosition(const vector3 &position) const { m_position->set(position); }

		inline void setPosition(const float &x, const float &y, const float &z) const { m_position->set(x, y, z); }

		inline vector3 *getRotation() const { return m_rotation; }

		inline void setRotation(const vector3 &rotation) const { m_rotation->set(rotation); }

		inline void setRotation(const float &x, const float &y, const float &z) const { m_rotation->set(x, y, z); }

		inline vector3 *getScaling() const { return m_scaling; }

		inline void setScaling(const vector3 &scaling) const { m_scaling->set(scaling); }

		inline void setScaling(const float &x, const float &y, const float &z) const { m_scaling->set(x, y, z); }

		inline void setScaling(const float &scale) const { m_scaling->set(scale, scale, scale); }
	};
}
