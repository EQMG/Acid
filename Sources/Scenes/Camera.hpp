#pragma once

#include "Scenes/Component.hpp"
#include "Maths/Matrix4.hpp"
#include "Maths/Vector3.hpp"
#include "Physics/Frustum.hpp"
#include "Physics/Ray.hpp"

namespace acid
{
/**
 * @brief Component that represents a scene camera, this object should be overridden with new behaviour.
 */
class ACID_EXPORT Camera :
	public Component
{
public:
	Camera() :
		m_nearPlane{0.1f},
		m_farPlane{1000.0f},
		m_fieldOfView{Maths::Radians(45.0f)},
		m_viewRay{false, Vector2f(0.5f, 0.5f)}
	{
	}

	virtual ~Camera() = default;

	/**
	 * Gets the distance of the near pane of the view frustum.
	 * @return The distance of the near pane of the view frustum.
	 */
	const float &GetNearPlane() const { return m_nearPlane; }

	void SetNearPlane(const float &nearPlane) { m_nearPlane = nearPlane; }

	/**
	 * Gets the distance of the view frustum's far plane.
	 * @return The distance of the view frustum's far plane.
	 */
	const float &GetFarPlane() const { return m_farPlane; }

	void SetFarPlane(const float &farPlane) { m_farPlane = farPlane; }

	/**
	 * Gets the field of view angle for the view frustum.
	 * @return The field of view angle for the view frustum.
	 */
	const float &GetFieldOfView() const { return m_fieldOfView; }

	void SetFieldOfView(const float &fieldOfView) { m_fieldOfView = fieldOfView; }

	const Vector3f &GetPosition() const { return m_position; }

	const Vector3f &GetRotation() const { return m_rotation; }

	const Vector3f &GetVelocity() const { return m_velocity; }

	/**
	 * Gets the view matrix created by the current camera position and rotation.
	 * @return The view matrix created by the current camera position and rotation.
	 */
	const Matrix4 &GetViewMatrix() const { return m_viewMatrix; }

	/**
	 * Gets the projection matrix used in the current scene render.
	 * @return The projection matrix used in the current scene render.
	 */
	const Matrix4 &GetProjectionMatrix() const { return m_projectionMatrix; }

	/**
	 * Gets the view frustum created by the current camera position and rotation.
	 * @return The view frustum created by the current camera position and rotation.
	 */
	const Frustum &GetViewFrustum() const { return m_viewFrustum; }

	/**
	 * Gets the ray that extends from the cameras position though the screen.
	 * @return The cameras view ray.
	 */
	const Ray &GetViewRay() const { return m_viewRay; }

protected:
	float m_nearPlane;
	float m_farPlane;
	float m_fieldOfView;

	Vector3f m_position;
	Vector3f m_rotation;
	Vector3f m_velocity;

	Matrix4 m_viewMatrix;
	Matrix4 m_projectionMatrix;

	Frustum m_viewFrustum;
	Ray m_viewRay;
};
}
