#pragma once

#include <Scenes/Camera.hpp>

using namespace acid;

namespace test
{
	class FixedCamera :
		public Camera
	{
	private:
		static const float NEAR_PLANE;
		static const float FAR_PLANE;
		static const float FIELD_OF_VIEW;

		Vector3 m_position;
		Vector3 m_velocity;
		Vector3 m_rotation;

		Matrix4 m_viewMatrix;
		Matrix4 m_projectionMatrix;

		Frustum m_viewFrustum;
		Ray m_viewRay;
	public:
		FixedCamera();

		void Start() override;

		void Update() override;

		float GetNearPlane() const override { return NEAR_PLANE; }

		float GetFarPlane() const override { return FAR_PLANE; }

		float GetFov() const override { return FIELD_OF_VIEW; }

		Frustum GetViewFrustum() const override { return m_viewFrustum; }

		Ray GetViewRay() const override { return m_viewRay; }

		Matrix4 GetViewMatrix() const override { return m_viewMatrix; }

		Matrix4 GetProjectionMatrix() const override { return m_projectionMatrix; }

		Vector3 GetPosition() const override { return m_position; }

		Vector3 GetVelocity() const override { return m_velocity; }

		Vector3 GetRotation() const override { return m_rotation; }
	};
}
