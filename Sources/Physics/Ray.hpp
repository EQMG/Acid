#pragma once

#include "Maths/Matrix4.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"

namespace acid
{
	/// <summary>
	/// Holds a 3 dimensional ray.
	/// </summary>
	class ACID_EXPORT Ray
	{
	public:
		static const Ray Zero;

		/// <summary>
		/// Creates a new 3D ray.
		/// </summary>
		/// <param name="useMouse"> If the ray will use the mouse coords or to start from screenStart. </param>
		/// <param name="screenStart"> If useMouse is false then this will be used as the rays start. </param>
		Ray(const bool &useMouse, const Vector2 &screenStart);

		/// <summary>
		/// Updates the ray to a new position.
		/// </summary>
		/// <param name="currentPosition"> The new position. </param>
		/// <param name="currentPosition"> The mouses xy screen space position. </param>
		/// <param name="viewMatrix"> The cameras view matrix. </param>
		/// <param name="projectionMatrix"> The projection view matrix. </param>
		void Update(const Vector3 &currentPosition, const Vector2 &mousePosition, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

		/// <summary>
		/// Gets a point on the ray.
		/// </summary>
		/// <param name="distance"> Distance down the ray to sample. </param>
		/// <returns> Returns the destination vector. </returns>
		Vector3 GetPointOnRay(const float &distance) const;

		/// <summary>
		/// Converts a position from world space to screen space.
		/// </summary>
		/// <param name="position"> The position to convert. </param>
		/// <returns> Returns the destination vector X and Y being screen space coords and Z being the distance to the camera. </returns>
		Vector3 ConvertToScreenSpace(const Vector3 &position) const;

		const bool &IsUseMouse() const { return m_useMouse; }

		void SetUseMouse(const bool &useMouse) { m_useMouse = useMouse; }

		const Vector2 &GetScreenStart() const { return m_screenStart; }

		void SetScreenStart(const Vector2 &screenStart) { m_screenStart = screenStart; }

		const Vector3 &GetOrigin() const { return m_origin; }

		const Vector3 &GetCurrentRay() const { return m_currentRay; }
	private:
		void UpdateNormalisedDeviceCoordinates(const float &mouseX, const float &mouseY);

		void UpdateEyeCoords();

		void UpdateWorldCoords();

		bool m_useMouse;
		Vector2 m_screenStart;

		Matrix4 m_viewMatrix;
		Matrix4 m_projectionMatrix;

		Vector2 m_normalizedCoords;
		Vector4 m_clipCoords;
		Vector4 m_eyeCoords;

		Matrix4 m_invertedProjection;
		Matrix4 m_invertedView;
		Vector4 m_rayWorld;

		Vector3 m_origin;
		Vector3 m_currentRay;
	};
}
