#pragma once

#include "vector3.hpp"
#include "matrix4x4.hpp"

namespace Flounder
{
	class Transform
	{
	private:
		vector3 *m_position;
		vector3 *m_rotation;
		vector3 *m_scaling;
	public:
		Transform();

		Transform(const Transform &source);

		Transform(const vector3 &position, const vector3 &rotation = vector3(), const vector3 &scaling = vector3(1.0f, 1.0f, 1.0f));

		~Transform();

		matrix4x4 *GetWorldMatrix(matrix4x4 *destination) const;

		matrix4x4 *GetModelMatrix(matrix4x4 *destination) const;

		void Set(const Transform &source) const;

		vector3 *GetPosition() const { return m_position; }

		void SetPosition(const vector3 &position) const { m_position->set(position); }

		void SetPosition(const float &x, const float &y, const float &z) const { m_position->set(x, y, z); }

		vector3 *GetRotation() const { return m_rotation; }

		void SetRotation(const vector3 &rotation) const { m_rotation->set(rotation); }

		void SetRotation(const float &x, const float &y, const float &z) const { m_rotation->set(x, y, z); }

		vector3 *GetScaling() const { return m_scaling; }

		void SetScaling(const vector3 &scaling) const { m_scaling->set(scaling); }

		void SetScaling(const float &x, const float &y, const float &z) const { m_scaling->set(x, y, z); }

		void SetScaling(const float &scale) const { m_scaling->set(scale, scale, scale); }
	};
}
