#include "FixedCamera.hpp"

#include <Scenes/Scenes.hpp>
#include <Inputs/Mouse.hpp>

namespace test
{
	const float FixedCamera::NEAR_PLANE = 0.1f;
	const float FixedCamera::FAR_PLANE = 10000.0f;
	const float FixedCamera::FIELD_OF_VIEW = 60.0f;

	FixedCamera::FixedCamera() :
		m_position(Vector3()),
		m_velocity(Vector3()),
		m_rotation(Vector3()),
		m_viewMatrix(Matrix4()),
		m_projectionMatrix(Matrix4()),
		m_viewFrustum(Frustum()),
		m_viewRay(Ray(false, Vector2(0.5f, 0.5f)))
	{
	}

	void FixedCamera::Start()
	{
	}

	void FixedCamera::Update()
	{
		m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation);
		m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFov(), Display::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

		m_viewFrustum.Update(m_viewMatrix, m_projectionMatrix);
		m_viewRay.Update(m_position, Vector2(Mouse::Get()->GetPositionX(), Mouse::Get()->GetPositionY()), m_viewMatrix, m_projectionMatrix);
	}
}
