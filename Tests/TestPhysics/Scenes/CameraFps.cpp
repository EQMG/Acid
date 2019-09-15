#include "CameraFps.hpp"

#include <Scenes/Scenes.hpp>
#include <Devices/Mouse.hpp>
#include <Maths/Maths.hpp>
#include <Graphics/Graphics.hpp>
#include <Physics/CollisionObject.hpp>
#include "PlayerFps.hpp"

namespace test
{
static const Vector3f VIEW_OFFSET(0.0f, 1.8f, 0.0f);
static const Vector2f SENSITIVITY_JOYSTICK(-0.06f);
static const Vector2f SENSITIVITY_MOUSE(0.15f);

CameraFps::CameraFps() :
	m_sensitivity(1.0f),
	m_joystickVertical(0, 3),
	m_joystickHorizontal(0, 2)
{
	m_nearPlane = 0.1f;
	m_farPlane = 4098.0f;
	m_fieldOfView = Maths::Radians(70.0f);
}

void CameraFps::Start()
{
}

void CameraFps::Update()
{
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	if (auto scenePlayer = Scenes::Get()->GetStructure()->GetComponent<PlayerFps>(); scenePlayer)
	{
		if (auto transformPlayer = scenePlayer->GetEntity()->GetComponent<Transform>(); transformPlayer)
		{
			m_velocity = (transformPlayer->GetPosition() - m_position) / delta;
			m_position = transformPlayer->GetPosition() + VIEW_OFFSET;
		}
	}

	if (!Scenes::Get()->IsPaused())
	{
		auto rotationDelta = Mouse::Get()->GetPositionDelta() * Mouse::Get()->IsCursorHidden() * SENSITIVITY_MOUSE;

		if (m_joystickVertical.IsConnected())
		{
			rotationDelta += Vector2f(m_joystickHorizontal.GetAmount(), m_joystickVertical.GetAmount()) * SENSITIVITY_JOYSTICK;
		}

		m_rotation.m_y += rotationDelta.m_x * m_sensitivity;
		m_rotation.m_x += rotationDelta.m_y * m_sensitivity;
		m_rotation.m_x = std::clamp(m_rotation.m_x, Maths::Radians(90.0f), Maths::Radians(270.0f));
	}

	m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation);
	m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView(), Window::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

	m_viewFrustum.Update(m_viewMatrix, m_projectionMatrix);
	m_viewRay.Update(m_position, {0.5f, 0.5f}, m_viewMatrix, m_projectionMatrix);

	//auto raytest = Scenes::Get()->GetPhysics()->Raytest(m_viewRay.GetOrigin(), m_viewRay.GetPointOnRay(20.0f));
	//Log::Out((raytest.HasHit() ? raytest.GetParent()->GetName() : ""), ": ", raytest.GetPointWorld().Distance(m_viewRay.GetOrigin()), '\n');
}
}
