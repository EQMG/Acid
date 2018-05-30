#include "FpsCamera.hpp"

#include <Scenes/Scenes.hpp>
#include <Inputs/Mouse.hpp>
#include <Maths/Maths.hpp>

namespace test
{
	const float FpsCamera::NEAR_PLANE = 0.1f;
	const float FpsCamera::FAR_PLANE = 10000.0f;
	const float FpsCamera::FIELD_OF_VIEW = 60.0f;

	FpsCamera::FpsCamera() :
		m_position(Vector3()),
		m_velocity(Vector3()),
		m_rotation(Vector3()),
		m_viewMatrix(Matrix4()),
		m_projectionMatrix(Matrix4()),
		m_viewFrustum(Frustum()),
		m_viewRay(Ray(false, Vector2(0.5f, 0.5f)))
	{
	}

	FpsCamera::~FpsCamera()
	{
	}

	void FpsCamera::Update()
	{
	//	float delta = std::min(1.0f / 60.0f, Engine::Get()->GetDelta());

		m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation);
		m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFov(), Display::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

		m_viewFrustum.Update(m_viewMatrix, m_projectionMatrix);
		m_viewRay.Update(m_position, Vector2(Mouse::Get()->GetPositionX(), Mouse::Get()->GetPositionY()), m_viewMatrix, m_projectionMatrix);
	}

	void FpsCamera::ReflectView(const float &waterHeight)
	{
		m_position.m_y -= 2.0f * (m_position.m_y - waterHeight);
		m_rotation.m_x = -m_rotation.m_x;
		m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation);
	}
}
