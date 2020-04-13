#pragma once

#include "Maths/Matrix4.hpp"
#include "Physics/Frustum.hpp"
#include "Physics/Ray.hpp"

namespace acid {
/**
 * @brief Component that represents a scene camera, this object should be overridden with new behaviour.
 */
class ACID_EXPORT Camera {
public:
	Camera() :
		nearPlane(0.1f),
		farPlane(1000.0f),
		fieldOfView(Maths::Radians(45.0f)),
		viewRay(false, {0.5f, 0.5f}) {
	}

	virtual ~Camera() = default;

	virtual void Start() {}
	virtual void Update() {}

	/**
	 * Gets the distance of the near pane of the view frustum.
	 * @return The distance of the near pane of the view frustum.
	 */
	float GetNearPlane() const { return nearPlane; }
	void SetNearPlane(float nearPlane) { this->nearPlane = nearPlane; }

	/**
	 * Gets the distance of the view frustum's far plane.
	 * @return The distance of the view frustum's far plane.
	 */
	float GetFarPlane() const { return farPlane; }
	void SetFarPlane(float farPlane) { this->farPlane = farPlane; }

	/**
	 * Gets the field of view angle for the view frustum.
	 * @return The field of view angle for the view frustum.
	 */
	float GetFieldOfView() const { return fieldOfView; }
	void SetFieldOfView(float fieldOfView) { this->fieldOfView = fieldOfView; }

	const Vector3f &GetPosition() const { return position; }
	const Vector3f &GetRotation() const { return rotation; }
	const Vector3f &GetVelocity() const { return velocity; }

	/**
	 * Gets the view matrix created by the current camera position and rotation.
	 * @return The view matrix created by the current camera position and rotation.
	 */
	const Matrix4 &GetViewMatrix() const { return viewMatrix; }

	/**
	 * Gets the projection matrix used in the current scene render.
	 * @return The projection matrix used in the current scene render.
	 */
	const Matrix4 &GetProjectionMatrix() const { return projectionMatrix; }

	/**
	 * Gets the view frustum created by the current camera position and rotation.
	 * @return The view frustum created by the current camera position and rotation.
	 */
	const Frustum &GetViewFrustum() const { return viewFrustum; }

	/**
	 * Gets the ray that extends from the cameras position though the screen.
	 * @return The cameras view ray.
	 */
	const Ray &GetViewRay() const { return viewRay; }

protected:
	float nearPlane, farPlane;
	float fieldOfView;

	Vector3f position;
	Vector3f rotation;
	Vector3f velocity;

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;

	Frustum viewFrustum;
	Ray viewRay;
};
}
