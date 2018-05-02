#pragma once

#include "Maths/Matrix4.hpp"

namespace fl
{
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

	/// <summary>
	/// Represents the region of flounder.space in the modeled world that may appear on the screen.
	/// </summary>
	class FL_EXPORT Frustum
	{
	private:
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
		void Update(const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix) const;

		/// <returns> The planes*value array used to represent the frustum. </returns>
		float **GetFrustum() const { return m_frustum; };

		/// <summary>
		/// Is the point contained in the frustum?
		/// </summary>
		/// <param name="position"> The points position. </param>
		/// <returns> True if contained, false if outside. </returns>
		bool PointInFrustum(const Vector3 &position) const;

		/// <summary>
		/// Is the sphere contained in the frustum?
		/// </summary>
		/// <param name="position"> The points position. </param>
		/// <param name="radius"> The spheres radius. </param>
		/// <returns> True if contained, false if outside. </returns>
		bool SphereInFrustum(const Vector3 &position, const float &radius) const;

		/// <summary>
		/// Is the cube contained partially in the frustum?
		/// </summary>
		/// <param name="min"> The point 1st position. </param>
		/// <param name="max"> The point 2nd position. </param>
		/// <returns> True if partially contained, false if outside. </returns>
		bool CubeInFrustum(const Vector3 &min, const Vector3 &max) const;

	private:
		void NormalizePlane(const int &side) const;
	};
}
