#pragma once

#include "Engine/Exports.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"

namespace fl
{
	/// <summary>
	/// Holds position, rotation, and scale components.
	/// </summary>
	class FL_EXPORT Transform
	{
	private:
		Vector3 *m_position;
		Vector3 *m_rotation;
		Vector3 *m_scaling;
	public:
		/// <summary>
		/// Constructor for Transform.
		/// </summary>
		Transform();

		/// <summary>
		/// Constructor for Transform.
		/// </summary>
		/// <param name="source"> Creates this vector out of a transform. </param>
		Transform(const Transform &source);

		/// <summary>
		/// Constructor for Transform.
		/// </summary>
		/// <param name="position"> The position. </param>
		/// <param name="rotation"> The rotation. </param>
		/// <param name="scaling"> The scaling. </param>
		Transform(const Vector3 &position, const Vector3 &rotation = Vector3(), const Vector3 &scaling = Vector3(1.0f, 1.0f, 1.0f));

		/// <summary>
		/// Constructor for Transform.
		/// </summary>
		/// <param name="position"> The position. </param>
		/// <param name="rotation"> The rotation. </param>
		/// <param name="scale"> The scale. </param>
		Transform(const Vector3 &position, const Vector3 &rotation, const float &scale);

		~Transform();

		Matrix4 GetWorldMatrix() const;

		Matrix4 GetModelMatrix() const;

		/// <summary>
		/// Saves this transform into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *destination);

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) { *m_position = position; }

		Vector3 *GetRotation() const { return m_rotation; }

		void SetRotation(const Vector3 &rotation) { *m_rotation = rotation; }

		Vector3 *GetScaling() const { return m_scaling; }

		void SetScaling(const Vector3 &scaling) { *m_scaling = scaling; }

		Transform &operator=(const Transform &other);

		Transform &operator=(LoadedValue *value);

		bool operator==(const Transform &other) const;

		bool operator!=(const Transform &other) const;
	};
}
