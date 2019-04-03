#pragma once

#include "Maths/Matrix4.hpp"

namespace acid
{
/**
 * @brief Represents the region of space in the modeled world that may appear on the screen.
 */
class ACID_EXPORT Frustum
{
public:
	/**
	 * Creates a new frustum.
	 */
	explicit Frustum();

	/**
	 * Updates a frustum from the view and projection matrix.
	 * @param view The view matrix.
	 * @param projection The projection matrix.
	 */
	void Update(const Matrix4 &view, const Matrix4 &projection);

	/**
	 * Gets if a point contained in the frustum.
	 * @param position The point.
	 * @return If the point is contained.
	 */
	bool PointInFrustum(const Vector3f &position) const;

	/**
	 * Gets if a sphere contained in the frustum.
	 * @param position The spheres position.
	 * @param radius The spheres radius.
	 * @return If the sphere is contained.
	 */
	bool SphereInFrustum(const Vector3f &position, const float &radius) const;

	/**
	 * Gets if a cube contained in the frustum.
	 * @param min The cube min point.
	 * @param max The cube max point.
	 * @return If cube sphere is contained.
	 */
	bool CubeInFrustum(const Vector3f &min, const Vector3f &max) const;

	static const Frustum Zero;

private:
	void NormalizePlane(const int32_t &side);

	std::array<std::array<float, 4>, 6> m_frustum;
};
}
