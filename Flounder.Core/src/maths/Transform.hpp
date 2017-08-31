#pragma once

#include "Vector3.hpp"
#include "Matrix4.hpp"

namespace Flounder
{
	class Transform
	{
	private:
		Vector3 *m_position;
		Vector3 *m_rotation;
		Vector3 *m_scaling;
	public:
		Transform();

		Transform(const Transform &source);

		Transform(const Vector3 &position, const Vector3 &rotation = Vector3(), const Vector3 &scaling = Vector3(1.0f, 1.0f, 1.0f));

		~Transform();

		Matrix4 *GetWorldMatrix(Matrix4 *destination) const;

		Matrix4 *GetModelMatrix(Matrix4 *destination) const;

		void Set(const Transform &source) const;

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) const { m_position->set(position); }

		void SetPosition(const float &x, const float &y, const float &z) const { m_position->set(x, y, z); }

		Vector3 *GetRotation() const { return m_rotation; }

		void SetRotation(const Vector3 &rotation) const { m_rotation->set(rotation); }

		void SetRotation(const float &x, const float &y, const float &z) const { m_rotation->set(x, y, z); }

		Vector3 *GetScaling() const { return m_scaling; }

		void SetScaling(const Vector3 &scaling) const { m_scaling->set(scaling); }

		void SetScaling(const float &x, const float &y, const float &z) const { m_scaling->set(x, y, z); }

		void SetScaling(const float &scale) const { m_scaling->set(scale, scale, scale); }
	};
}
