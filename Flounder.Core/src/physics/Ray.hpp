#pragma once

#include "../maths/Vector2.hpp"
#include "../maths/Vector3.hpp"
#include "../maths/Vector4.hpp"
#include "../maths/Matrix4.hpp"

namespace Flounder
{
	/// <summary>
	/// Holds a 3 dimensional ray.
	/// </summary>
	class ray
	{
	private:
		bool m_useMouse;
		Vector2 *m_screenStart;

		Matrix4 *m_viewMatrix;
		Matrix4 *m_projectionMatrix;

		Vector2 *m_normalizedCoords;
		Vector4 *m_clipCoords;
		Vector4 *m_eyeCoords;

		Matrix4 *m_invertedProjection;
		Matrix4 *m_invertedView;
		Vector4 *m_rayWorld;
	public:
		Vector3 *m_origin;
		Vector3 *m_currentRay;

		/// <summary>
		/// Creates a new 3D ray.
		/// </summary>
		/// <param name="useMouse"> If the ray will use the mouse coords or to start from screenStart. </param>
		/// <param name="screenStart"> If useMouse is false then this will be used as the rays start. </param>
		ray(const bool &useMouse, const Vector2 &screenStart);

		/// <summary>
		/// Deconstructor for the ray.
		/// </summary>
		~ray();

		/// <summary>
		/// Updates the ray to a new position.
		/// </summary>
		/// <param name="currentPosition"> The new position. </param>
		/// <param name="currentPosition"> The mouses xy screen space position. </param>
		/// <param name="viewMatrix"> The cameras view matrix. </param>
		/// <param name="projectionMatrix"> The projection view matrix. </param>
		void update(const Vector3 &currentPosition, const Vector2 &mousePosition, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

		/// <summary>
		/// Gets a point on the ray.
		/// </summary>
		/// <param name="distance"> Distance down the ray to sample. </param>
		/// <param name="destination"> The destination vector, if nullptr one will be created. </param>
		/// <returns> Returns the destination vector. </returns>
		Vector3 *getPointOnRay(const float &distance, Vector3 *destination) const;

		/// <summary>
		/// Converts a position from world space to screen space.
		/// </summary>
		/// <param name="position"> The position to convert. </param>
		/// <param name="destination"> The destination point. X and Y being screen space coords and Z being the distance to the camera. </param>
		/// <returns> Returns the destination vector. </returns>
		Vector3 *convertToScreenSpace(const Vector3 &position, Vector3 *destination) const;
	private:
		void updateNormalisedDeviceCoordinates(const float &mouseX, const float &mouseY);

		void updateEyeCoords(Vector4 *clipCoords);

		void updateWorldCoords(Vector4 *eyeCoords);
	};
}
