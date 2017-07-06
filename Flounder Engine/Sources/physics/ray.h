#pragma once

#include "../maths/vector2.h"
#include "../maths/vector3.h"
#include "../maths/vector4.h"
#include "../maths/matrix4x4.h"

namespace flounder {
	/// <summary>
	/// Holds a 3 dimensional ray.
	/// </summary>
	class ray
	{
	private:
		bool m_useMouse;
		vector2 *m_screenStart;

		matrix4x4 *m_viewMatrix;
		matrix4x4 *m_projectionMatrix;

		vector3 *m_origin;
		vector3 *m_currentRay;

		vector2 *m_normalizedCoords;
		vector4 *m_clipCoords;
		vector4 *m_eyeCoords;

		matrix4x4 *m_invertedProjection;
		matrix4x4 *m_invertedView;
		vector4 *m_rayWorld;

	public:
		/// <summary>
		/// Creates a new 3D ray.
		/// </summary>
		/// <param name="useMouse"> If the ray will use the mouse coords or to start from screenStart. </param>
		/// <param name="screenStart"> If useMouse is false then this will be used as the rays start. </param>
		ray(bool useMouse, vector2 *screenStart);

		/// <summary>
		/// Deconstructor for the ray.
		/// </summary>
		~ray();

		/// <summary>
		/// Updates the ray to a new position.
		/// </summary>
		/// <param name="currentPosition"> The new position. </param
		/// <param name="currentPosition"> The mouses xy screen space position. </param>
		/// <param name="viewMatrix"> The cameras view matrix. </param>
		/// <param name="projectionMatrix"> The projection view matrix. </param>
		void update(vector3 *currentPosition, vector2 *mousePosition, matrix4x4 *viewMatrix, matrix4x4 *projectionMatrix);

		/// <summary>
		/// Gets a point on the ray.
		/// </summary>
		/// <param name="distance"> Distance down the ray to sample. </param>
		/// <param name="destination"> The destination vector, if null one will be created.
		/// </param>
		/// <returns> Returns the destination vector. </returns>
		vector3 *getPointOnRay(float distance, vector3 *destination);

		/// <summary>
		/// Converts a position from world space to screen space.
		/// </summary>
		/// <param name="position"> The position to convert. </param>
		/// <param name="destination"> The destination point. X and Y being screen space coords and Z being the distance to the camera.
		/// </param>
		/// <returns> Returns the destination vector. </returns>
		vector3 *convertToScreenSpace(vector3 *position, vector3 *destination);

		/// <summary>
		/// Gets the rays origin.
		/// </summary>
		/// <returns> The rays origin. </returns>
		inline vector3 *getOrigin() { return m_origin; };

		/// <summary>
		/// Gets the current ray.
		/// </summary>
		/// <returns> The current ray. </returns>
		inline vector3 *getCurrentRay() { return m_currentRay; };
	private:
		void updateNormalisedDeviceCoordinates(float mouseX, float mouseY);

		void updateEyeCoords(vector4 *clipCoords);

		void updateWorldCoords(vector4 *eyeCoords);
	};
}
