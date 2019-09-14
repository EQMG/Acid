#pragma once

#include "Maths/Matrix4.hpp"
#include "Maths/Vector4.hpp"
#include "Scenes/Camera.hpp"

namespace acid
{
/**
 * @brief Represents the 3D area of the world in which engine.shadows will be cast (basically represents the orthographic projection area for the shadow render pass).
 * It can be updated each frame to optimize the area, making it as small as possible (to allow for optimal shadow map resolution) while not being too small to avoid objects not having shadows when they should.
 * This class also provides functionality to test whether an object is inside this shadow box. Everything inside the box will be rendered to the shadow map in the shadow render pass.
 */
class ACID_EXPORT ShadowBox
{
public:
	/**
	 * Creates a new shadow box and calculates some initial values relating to the camera's view frustum.
	 */
	ShadowBox();

	/**
	 * Updates the bounds of the shadow box based on the light direction and the camera's view frustum.
	 * Will make sure that the box covers the smallest area possible while still ensuring that everything.
	 * Objects inside the camera's view (and in range) will be shadowed.
	 * @param camera The camera object to be used when calculating the shadow boxes size.
	 * @param lightDirection The lights direction.
	 * @param shadowOffset The shadows offset.
	 * @param shadowDistance The shadows distance.
	 */
	void Update(const Camera &camera, const Vector3f &lightDirection, float shadowOffset, float shadowDistance);

	/**
	 * Tests if a bounding sphere intersects the shadow box. Can be used to decide which engine.entities should be rendered in the shadow render pass.
	 * @param position The centre of the bounding sphere in world space.
	 * @param radius The radius of the bounding sphere.
	 * @return {@code true} if the sphere intersects the box.
	 */
	bool IsInBox(const Vector3f &position, float radius) const;

	const Matrix4 &GetProjectionViewMatrix() const { return m_projectionViewMatrix; }

	/**
	 * This biased projection-view matrix is used to convert fragments into "shadow map space" when rendering the main render pass.
	 * @return The to-shadow-map-space matrix.
	 */
	const Matrix4 &GetToShadowMapSpaceMatrix() const { return m_shadowMapSpaceMatrix; }

	/**
	 * Gets the light's "view" matrix
	 * @return The light's "view" matrix.
	 */
	const Matrix4 &GetLightSpaceTransform() const { return m_lightViewMatrix; }

	const Vector3f &GetMinExtents() const { return m_minExtents; }

	const Vector3f &GetMaxExtents() const { return m_maxExtents; }

	float GetWidth() const { return m_maxExtents.m_x - m_minExtents.m_x; }

	float GetHeight() const { return m_maxExtents.m_y - m_minExtents.m_y; }

	float GetDepth() const { return m_maxExtents.m_z - m_minExtents.m_z; }

private:
	void UpdateShadowBox(const Camera &camera);

	/**
	 * Updates the widths and heights of the box panes.
	 * @param camera The camera object.
	 */
	void UpdateSizes(const Camera &camera);

	/**
	 * Calculates the vertex of each corner of the view frustum in light space.
	 * @param rotation The cameras rotation.
	 * @param forwardVector The direction that the camera is aiming, and thus the direction of the frustum.
	 * @param centreNear The centre point of the frustum's near plane.
	 * @param centreFar The centre point of the frustum's far plane.
	 * @return The vertices of the frustum in light space.
	 */
	std::array<Vector4f, 8> CalculateFrustumVertices(const Matrix4 &rotation, const Vector3f &forwardVector, const Vector3f &centreNear, const Vector3f &centreFar) const;

	/**
	 * Calculates one of the corner vertices of the view frustum in world space and converts it to light space.
	 * @param startPoint The starting centre point on the view frustum.
	 * @param direction The direction of the corner from the start point.
	 * @param width The distance of the corner from the start point.
	 * @return The relevant corner vertex of the view frustum in light space.
	 */
	Vector4f CalculateLightSpaceFrustumCorner(const Vector3f &startPoint, const Vector3f &direction, float width) const;

	void UpdateOrthoProjectionMatrix();

	/**
	 * Updates the centre of the shadow box (orthographic projection area).
	 */
	void UpdateCenter();

	void UpdateLightViewMatrix();

	void UpdateViewShadowMatrix();

	Vector3f m_lightDirection;
	float m_shadowOffset = 0.0f;
	float m_shadowDistance = 0.0f;;

	Matrix4 m_projectionMatrix;
	Matrix4 m_lightViewMatrix;
	Matrix4 m_projectionViewMatrix;
	Matrix4 m_shadowMapSpaceMatrix;
	Matrix4 m_offset;
	Vector3f m_centre;

	float m_farHeight = 0.0f, m_farWidth = 0.0f;
	float m_nearHeight = 0.0f, m_nearWidth = 0.0f;

	Vector3f m_minExtents;
	Vector3f m_maxExtents;
};
}
