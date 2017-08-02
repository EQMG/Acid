#pragma once

#include "../camera/icamera.h"
#include "../devices/display.h"
#include "../maths/vector4.h"
#include "../maths/matrix4x4.h"
#include "../physics/aabb.h"

namespace flounder
{
	/// <summary>
	/// Represents the 3D area of the world in which engine.shadows will be cast (basically represents the orthographic projection area for the shadow render pass).
	/// It can be updated each frame to optimise the area, making it as small as possible (to allow for optimal shadow map resolution) while not being too small to avoid objects not having shadows when they should.
	/// This class also provides functionality to test whether an object is inside this shadow box. Everything inside the box will be rendered to the shadow map in the shadow render pass.
	/// </summary>
	class shadowbox
	{
	private:
		static vector4 const UP;
		static vector4 const FORWARD;

		matrix4x4 *m_lightViewMatrix;
		aabb *m_aabb;

		float m_shadowDistance, m_shadowOffset;
		float m_farHeight, m_farWidth;
		float m_nearHeight, m_nearWidth;
	public:
		/// <summary>
		/// Creates a new shadow box and calculates some initial values relating to the camera's view frustum.
		/// </summary>
		/// <param name="lightViewMatrix"> Basically the "view matrix" of the light. Can be used to transform a point from world space into "light" space. </param>
		shadowbox(const matrix4x4 &lightViewMatrix);

		~shadowbox();

		/// <summary>
		/// Updates the bounds of the shadow box based on the light direction and the camera's view frustum.
		/// Will make sure that the box covers the smallest area possible while still ensuring that everything.
		/// Objects inside the camera's view (and in range) will be shadowed.
		/// </summary>
		/// <param name="camera"> The camera object to be used when calculating the shadow boxes size. </param>
		/// <param name="shadowDistance"> The shadows distance. </param>
		/// <param name="shadowOffset"> The shadows offset. </param>
		void update(const icamera &camera, const float &shadowDistance, const float &shadowOffset);
	private:
		/// <summary>
		/// Updates the widths and heights of the box panes.
		/// </summary>
		/// <param name="camera"> The camera object. </param>
		void updateWidthsAndHeights(const icamera &camera);

		/// <summary>
		/// Calculates the rotation of the camera represented as a matrix.
		/// </summary>
		/// <returns> The rotation of the camera represented as a matrix. </returns>
		matrix4x4 *calculateCameraRotationMatrix(const icamera &camera);

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
	public:
		/// <summary>
		/// Test if a bounding sphere intersects the shadow box. Can be used to decide which engine.entities should be rendered in the shadow render pass.
		/// </summary>
		/// <param name="position"> The centre of the bounding sphere in world space. </param>
		/// <param name="radius"> The radius of the bounding sphere.
		/// </param>
		/// <returns> {@code true} if the sphere intersects the box. </returns>
		bool isInBox(const vector3 &position, const float &radius);

		/// <summary>
		/// Gets the centre of the shadow box (orthographic projection area).
		/// </summary>
		/// <returns> The centre of the shadow box. </returns>
		vector3 *getCenter();

		/// <summary>
		/// Gets the width of the shadow box (orthographic projection area).
		/// </summary>
		/// <returns> The width of the shadow box. </returns>
		float getWidth();

		/// <summary>
		/// Gets the height of the shadow box (orthographic projection area).
		/// </summary>
		/// <returns> The height of the shadow box. </returns>
		float getHeight();

		/// <summary>
		/// Gets the length of the shadow box (orthographic projection area).
		/// </summary>
		/// <returns> The length of the shadow box. </returns>
		float getLength();

		aabb *getAabb() const { return m_aabb; }
	};
}
