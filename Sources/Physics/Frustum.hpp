#pragma once

#include "Maths/Matrix4.hpp"

namespace fl
{
	enum FrustumPlanes
	{
		FRUSTUM_RIGHT = 0,
		FRUSTUM_LEFT = 1,
		FRUSTUM_BOTTOM = 2,
		FRUSTUM_TOP = 3,
		FRUSTUM_BACK = 4,
		FRUSTUM_FRONT = 5
	};

	enum FrustumFace
	{
		FRUSTUM_A = 0,
		FRUSTUM_B = 1,
		FRUSTUM_C = 2,
		FRUSTUM_D = 3
	};

	/// <summary>
	/// Represents the region of flounder.space in the modeled world that may appear on the screen.
	/// </summary>
	class FL_EXPORT Frustum
	{
	private:
		float **m_frustumArray; // TODO: Move to using a `std::array` instead.
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
		float **GetFrustumArray() const { return m_frustumArray; };

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
