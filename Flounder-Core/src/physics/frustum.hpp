#pragma once

#include "../helpers/helperarray.hpp"
#include "../maths/matrix4x4.hpp"

namespace flounder
{
	/// <summary>
	/// Represents the region of flounder.space in the modeled world that may appear on the screen.
	/// </summary>
	class frustum
	{
	private:
		// Each frustum planes.
		static const int RIGHT;
		static const int LEFT;
		static const int BOTTOM;
		static const int TOP;
		static const int BACK;
		static const int FRONT;

		// The values stored in the planes.
		static const int A;
		static const int B;
		static const int C;
		static const int D;

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
		void update(const matrix4x4 &viewMatrix, const matrix4x4 &projectionMatrix);

		/// <returns> The planes*value array used to represent the frustum. </returns>
		inline float **getFrustum() const { return m_frustum; };

		/// <summary>
		/// Is the point contained in the frustum?
		/// </summary>
		/// <param name="x"> The points X coord. </param>
		/// <param name="y"> The points Y coord. </param>
		/// <param name="z"> The points Z coord. </param>
		/// <returns> True if contained, false if outside. </returns>
		bool pointInFrustum(const float &x, const float &y, const float &z) const;

		/// <summary>
		/// Is the sphere contained in the frustum?
		/// </summary>
		/// <param name="x"> The sphere X coord. </param>
		/// <param name="y"> The sphere Y coord. </param>
		/// <param name="z"> The sphere Z coord. </param>
		/// <param name="radius"> The spheres radius. </param>
		/// <returns> True if contained, false if outside. </returns>
		bool sphereInFrustum(const float &x, const float &y, const float &z, const float &radius) const;

		/// <summary>
		/// Is the cube contained partially in the frustum?
		/// </summary>
		/// <param name="x1"> The point 1's X coord. </param>
		/// <param name="y1"> The point 1's Y coord. </param>
		/// <param name="z1"> The point 1's Z coord. </param>
		/// <param name="x2"> The point 2's X coord. </param>
		/// <param name="y2"> The point 2's Y coord. </param>
		/// <param name="z2"> The point 2's Z coord. </param>
		/// <returns> True if partially contained, false if outside. </returns>
		bool cubeInFrustum(const float &x1, const float &y1, const float &z1, const float &x2, const float &y2, const float &z2) const;
	private:
		void normalizePlane(float **frustum, const int &side);
	};
}
