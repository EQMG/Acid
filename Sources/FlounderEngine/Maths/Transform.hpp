#pragma once

#include "../Prerequisites.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"

namespace Flounder
{
	class F_EXPORT Transform
	{
	private:
	public:
		Vector3 *m_position;
		Vector3 *m_rotation;
		Vector3 *m_scaling;

		Transform();

		Transform(const Transform &source);

		Transform(const Vector3 &position, const Vector3 &rotation = Vector3(), const Vector3 &scaling = Vector3(1.0f, 1.0f, 1.0f));

		Transform(const Vector3 &position, const Vector3 &rotation, const float &scale);

		~Transform();

		Matrix4 *GetWorldMatrix(Matrix4 *destination) const;

		Matrix4 *GetModelMatrix(Matrix4 *destination) const;

		void Set(const Transform &source) const;

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) const { m_position->Set(position); }

		Vector3 *GetRotation() const { return m_rotation; }

		void SetRotation(const Vector3 &rotation) const { m_rotation->Set(rotation); }

		Vector3 *GetScaling() const { return m_scaling; }

		void SetScaling(const Vector3 &scaling) const { m_scaling->Set(scaling); }

		bool operator==(const Transform &other) const;

		bool operator!=(const Transform &other) const;
	};
}
