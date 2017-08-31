#pragma once

#include "../camera/icamera.hpp"
#include "../devices/display.hpp"
#include "../maths/vector4.hpp"
#include "../maths/matrix4x4.hpp"
#include "../physics/aabb.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents the 3D area of the world in which engine.shadows will be cast (basically represents the orthographic projection area for the shadow render pass).
	/// It can be updated each frame to optimise the area, making it as small as possible (to allow for optimal shadow map resolution) while not being too small to avoid objects not having shadows when they should.
	/// This class also provides functionality to test whether an object is inside this shadow box. Everything inside the box will be rendered to the shadow map in the shadow render pass.
	/// </summary>
	class shadowbox
	{
	private:
		vector3 *m_lightDirection;
		float m_shadowOffset;
		float m_shadowDistance;

		matrix4x4 *m_projectionMatrix;
		matrix4x4 *m_lightViewMatrix;
		matrix4x4 *m_projectionViewMatrix;
		matrix4x4 *m_shadowMapSpaceMatrix;
		matrix4x4 *m_offset;
		vector3 *m_centre;

		float m_farHeight, m_farWidth;
		float m_nearHeight, m_nearWidth;

		aabb *m_aabb;
	public:
		/// <summary>
		/// Creates a new shadow box and calculates some initial values relating to the camera's view frustum.
		/// </summary>
		shadowbox();

		~shadowbox();

		/// <summary>
		/// Updates the bounds of the shadow box based on the light direction and the camera's view frustum.
		/// Will make sure that the box covers the smallest area possible while still ensuring that everything.
		/// Objects inside the camera's view (and in range) will be shadowed.
		/// </summary>
		/// <param name="camera"> The camera object to be used when calculating the shadow boxes size. </param>
		/// <param name="lightPosition"> The lights position. </param>
		/// <param name="shadowOffset"> The shadows offset. </param>
		/// <param name="shadowDistance"> The shadows distance. </param>
		void update(const ICamera &camera, const vector3 &lightPosition, const float &shadowOffset, const float &shadowDistance);
	private:
		/// <summary>
		/// Create the offset for part of the conversion to shadow map space.
		/// </summary>
		/// <returns> The offset as a matrix. </returns>
		static matrix4x4 *createOffset();

		void updateShadowBox(const ICamera &camera);

		/// <summary>
		/// Updates the widths and heights of the box panes.
		/// </summary>
		/// <param name="camera"> The camera object. </param>
		void updateWidthsAndHeights(const ICamera &camera);

		/// <summary>
		/// Calculates the vertex of each corner of the view frustum in light space.
		/// </summary>
		/// <param name="rotation"> - camera's rotation. </param>
		/// <param name="forwardVector"> - the direction that the camera is aiming, and thus the direction of the frustum. </param>
		/// <param name="centreNear"> - the centre point of the frustum's near plane. </param>
		/// <param name="centreFar"> - the centre point of the frustum's far plane.
		/// </param>
		/// <returns> The vertices of the frustum in light space. </returns>
		vector4 **calculateFrustumVertices(const matrix4x4 &rotation, const vector3 &forwardVector, const vector3 &centreNear, const vector3 &centreFar);

		/// <summary>
		/// Calculates one of the corner vertices of the view frustum in world space and converts it to light space.
		/// </summary>
		/// <param name="startPoint"> The starting centre point on the view frustum. </param>
		/// <param name="direction"> The direction of the corner from the start point. </param>
		/// <param name="width"> The distance of the corner from the start point.
		/// </param>
		/// <returns> The relevant corner vertex of the view frustum in light space. </returns>
		vector4 *calculateLightSpaceFrustumCorner(const vector3 &startPoint, const vector3 &direction, const float &width);

		/// <summary>
		/// Updates the centre of the shadow box (orthographic projection area).
		/// </summary>
		void updateCenter();

		void updateOrthoProjectionMatrix();

		void updateLightViewMatrix();

		void updateViewShadowMatrix();
	public:
		/// <summary>
		/// Test if a bounding sphere intersects the shadow box. Can be used to decide which engine.entities should be rendered in the shadow render pass.
		/// </summary>
		/// <param name="position"> The centre of the bounding sphere in world space. </param>
		/// <param name="radius"> The radius of the bounding sphere.
		/// </param>
		/// <returns> {@code true} if the sphere intersects the box. </returns>
		bool isInBox(const vector3 &position, const float &radius);

		matrix4x4 *getProjectionViewMatrix() const { return m_projectionViewMatrix; }

		/// <summary>
		/// This biased projection-view matrix is used to convert fragments into "shadow map space" when rendering the main render pass.
		/// </summary>
		/// <returns> The to-shadow-map-space matrix. </returns>
		matrix4x4 *getToShadowMapSpaceMatrix() const { return m_shadowMapSpaceMatrix; }

		/// <summary>
		/// Gets the light's "view" matrix
		/// </summary>
		/// <returns> The light's "view" matrix. </returns>
		matrix4x4 *getLightSpaceTransform() const { return m_lightViewMatrix; }

		aabb *getAabb() const { return m_aabb; }
	};
}
