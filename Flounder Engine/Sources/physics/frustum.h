#pragma once

#include "../helpers/helperarray.h"
#include "../maths/matrix4x4.h"

namespace flounder {
	/// <summary>
	/// Represents the region of flounder.space in the modeled world that may appear on the screen.
	/// </summary>
	class frustum
	{
	private:
		// Each frustum planes.
		static const int RIGHT = 0;
		static const int LEFT = 1;
		static const int BOTTOM = 2;
		static const int TOP = 3;
		static const int BACK = 4;
		static const int FRONT = 5;

		// The values stored in the planes.
		static const int A = 0;
		static const int B = 1;
		static const int C = 2;
		static const int D = 3;

		float **m_frustum;

	public:
		/// <summary>
		/// Creates a new frustum.
		/// </summary>
		frustum();

		/// <summary>
		/// Deconstructor for frustum.
		/// </summary>
		~frustum();

		/// <summary>
		/// Updates a frustum from the view and projection matrix.
		/// </summary>
		/// <param name="viewMatrix"> The view matrix. </param>
		/// <param name="projectionMatrix"> The projection matrix. </param>
		void update(matrix4x4 *viewMatrix, matrix4x4 *projectionMatrix);

		/// <returns> The planes*value array used to represent the frustum. </returns>
		inline float **getFrustum() { return m_frustum; };

		/// <summary>
		/// Is the point contained in the frustum?
		/// </summary>
		/// <param name="x"> The points X coord. </param>
		/// <param name="y"> The points Y coord. </param>
		/// <param name="z"> The points Z coord.
		/// </param>
		/// <returns> True if contained, false if outside. </returns>
		bool pointInFrustum(float x, float y, float z);

		/// <summary>
		/// Is the sphere contained in the frustum?
		/// </summary>
		/// <param name="x"> The sphere X coord. </param>
		/// <param name="y"> The sphere Y coord. </param>
		/// <param name="z"> The sphere Z coord. </param>
		/// <param name="radius"> The spheres radius.
		/// </param>
		/// <returns> True if contained, false if outside. </returns>
		bool sphereInFrustum(float x, float y, float z, float radius);

		/// <summary>
		/// Is the cube contained partially in the frustum?
		/// </summary>
		/// <param name="x1"> The point 1's X coord. </param>
		/// <param name="y1"> The point 1's Y coord. </param>
		/// <param name="z1"> The point 1's Z coord. </param>
		/// <param name="x2"> The point 2's X coord. </param>
		/// <param name="y2"> The point 2's Y coord. </param>
		/// <param name="z2"> The point 2's Z coord.
		/// </param>
		/// <returns> True if partially contained, false if outside. </returns>
		bool cubeInFrustum(float x1, float y1, float z1, float x2, float y2, float z2);
	private:
		void normalizePlane(float **frustum, int side);
	};
}
