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
		m_position(new Vector3()),
		m_velocity(new Vector3()),
		m_rotation(new Vector3()),
		m_viewMatrix(new Matrix4()),
		m_projectionMatrix(new Matrix4()),
		m_viewFrustum(new Frustum()),
		m_viewRay(new Ray(false, Vector2(0.5f, 0.5f)))
	{
	}

	FpsCamera::~FpsCamera()
	{
		delete m_position;
		delete m_velocity;
		delete m_rotation;

		delete m_viewMatrix;
		delete m_projectionMatrix;

		delete m_viewFrustum;
		delete m_viewRay;
	}

	void FpsCamera::Update()
	{
	//	float delta = std::min(1.0f / 60.0f, Engine::Get()->GetDelta());

		*m_viewMatrix = Matrix4::ViewMatrix(*m_position, *m_rotation);
		*m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFov(), Display::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

		m_viewFrustum->Update(*m_viewMatrix, *m_projectionMatrix);
		m_viewRay->Update(*m_position, Vector2(Mouse::Get()->GetPositionX(), Mouse::Get()->GetPositionY()), *m_viewMatrix, *m_projectionMatrix);
	}

	void FpsCamera::ReflectView(const float &waterHeight)
	{
		m_position->m_y -= 2.0f * (m_position->m_y - waterHeight);
		m_rotation->m_x = -m_rotation->m_x;
		*m_viewMatrix = Matrix4::ViewMatrix(*m_position, *m_rotation);
	}
}
