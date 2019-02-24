#pragma once

#include "Engine/Exports.hpp"
#include "Scenes/Component.hpp"
#include "Maths/Matrix4.hpp"
#include "Maths/Vector3.hpp"
#include "Physics/Frustum.hpp"
#include "Physics/Ray.hpp"

namespace acid
{
	/// <summary>
	/// This component represents a scene camera, this object should be overridden with new behaviour.
	/// </summary>
	class ACID_EXPORT Camera :
		public Component
	{
	public:
		Camera() :
			m_nearPlane(0.1f),
			m_farPlane(1000.0f),
			m_fieldOfView(45.0f),
			m_viewRay(false, Vector2(0.5f, 0.5f))
		{
		}

		virtual ~Camera() = default;

		/// <summary>
		/// Gets the distance of the near pane of the view frustum.
		/// </summary>
		/// <returns> The distance of the near pane of the view frustum. </returns>
		const float &GetNearPlane() const { return m_nearPlane; }

		void SetNearPlane(const float &nearPlane) { m_nearPlane = nearPlane; }

		/// <summary>
		/// Gets the distance of the view frustum's far plane.
		/// </summary>
		/// <returns> The distance of the view frustum's far plane. </returns>
		const float &GetFarPlane() const { return m_farPlane; }

		void SetFarPlane(const float &farPlane) { m_farPlane = farPlane; }

		/// <summary>
		/// Gets the field of view angle for the view frustum.
		/// </summary>
		/// <returns> The field of view angle for the view frustum. </returns>
		const float &GetFieldOfView() const { return m_fieldOfView; }

		void SetFieldOfView(const float &fieldOfView) { m_fieldOfView = fieldOfView; }

		/// <summary>
		/// Gets the cameras 3D position in the world.
		/// </summary>
		/// <returns> The cameras 3D position in the world. </returns>
		const Vector3 &GetPosition() const { return m_position; }

		/// <summary>
		/// Gets the cameras 3D rotation in the world, where x=pitch, y=yaw, z=roll.
		/// </summary>
		/// <returns> The cameras 3D rotation in the world, where x=pitch, y=yaw, z=roll. </returns>
		const Vector3 &GetRotation() const { return m_rotation; }

		/// <summary>
		/// Gets the cameras 3D velocity in the world.
		/// </summary>
		/// <returns> The cameras 3D velocity in the world. </returns>
		const Vector3 &GetVelocity() const { return m_velocity; }

		/// <summary>
		/// Gets the view matrix created by the current camera position and rotation.
		/// </summary>
		/// <returns> The view matrix created by the current camera position and rotation. </returns>
		const Matrix4 &GetViewMatrix() const { return m_viewMatrix; }

		/// <summary>
		/// Gets the projection matrix used in the current scene render.
		/// </summary>
		/// <returns> The projection matrix used in the current scene render. </returns>
		const Matrix4 &GetProjectionMatrix() const { return m_projectionMatrix; }

		/// <summary>
		/// Gets the view frustum created by the current camera position and rotation.
		/// </summary>
		/// <returns> The view frustum created by the current camera position and rotation. </returns>
		const Frustum &GetViewFrustum() const { return m_viewFrustum; }

		/// <summary>
		/// Gets the ray that extends from the cameras position though the screen.
		/// </summary>
		/// <returns> The cameras view ray. </returns>
		const Ray &GetViewRay() const { return m_viewRay; }
	protected:
		float m_nearPlane;
		float m_farPlane;
		float m_fieldOfView;

		Vector3 m_position;
		Vector3 m_rotation;
		Vector3 m_velocity;

		Matrix4 m_viewMatrix;
		Matrix4 m_projectionMatrix;

		Frustum m_viewFrustum;
		Ray m_viewRay;
	};
}
