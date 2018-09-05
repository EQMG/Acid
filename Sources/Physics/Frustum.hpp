#pragma once

#include <array>
#include "Maths/Matrix4.hpp"

namespace acid
{
	/// <summary>
	/// Represents the region of space in the modeled world that may appear on the screen.
	/// </summary>
	class ACID_EXPORT Frustum
	{
	private:
		std::array<std::array<float, 4>, 6> m_frustum;
	public:
		/// <summary>
		/// Creates a new frustum.
		/// </summary>
		Frustum();

		~Frustum();

		/// <summary>
		/// Updates a frustum from the view and projection matrix.
		/// </summary>
		/// <param name="view"> The view matrix. </param>
		/// <param name="projection"> The projection matrix. </param>
		void Update(const Matrix4 &view, const Matrix4 &projection);

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
		void NormalizePlane(const int32_t &side);
	};
}
