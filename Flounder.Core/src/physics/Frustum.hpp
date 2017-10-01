#pragma once

#include "../maths/Matrix4.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents the region of flounder.space in the modeled world that may appear on the screen.
	/// </summary>
	class Frustum
	{
	private:
		enum FrustumPlanes
		{
			FrustumRight = 0,
			FrustumLeft = 1,
			FrustumBottom = 2,
			FrustumTop = 3,
			FrustumBack = 4,
			FrustumFront = 5
		};

		enum FrustumFace
		{
			FrustumA = 0,
			FrustumB = 1,
			FrustumC = 2,
			FrustumD = 3
		};

		float **m_frustum;
	public:
		/// <summary>
		/// Creates a new frustum.
		/// </summary>
		Frustum();

		/// <summary>
		/// Deconstructor for frustum.
		/// </summary>
		~Frustum();

		/// <summary>
		/// Updates a frustum from the view and projection matrix.
		/// </summary>
		/// <param name="viewMatrix"> The view matrix. </param>
		/// <param name="projectionMatrix"> The projection matrix. </param>
		void Update(const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

		/// <returns> The planes*value array used to represent the frustum. </returns>
		float **GetFrustum() const { return m_frustum; };

		/// <summary>
		/// Is the point contained in the frustum?
		/// </summary>
		/// <param name="x"> The points X coord. </param>
		/// <param name="y"> The points Y coord. </param>
		/// <param name="z"> The points Z coord. </param>
		/// <returns> True if contained, false if outside. </returns>
		bool PointInFrustum(const float &x, const float &y, const float &z) const;

		/// <summary>
		/// Is the sphere contained in the frustum?
		/// </summary>
		/// <param name="x"> The sphere X coord. </param>
		/// <param name="y"> The sphere Y coord. </param>
		/// <param name="z"> The sphere Z coord. </param>
		/// <param name="radius"> The spheres radius. </param>
		/// <returns> True if contained, false if outside. </returns>
		bool SphereInFrustum(const float &x, const float &y, const float &z, const float &radius) const;

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
		bool CubeInFrustum(const float &x1, const float &y1, const float &z1, const float &x2, const float &y2, const float &z2) const;
	private:
		void NormalizePlane(float **frustum, const int &side) const;
	};
}
