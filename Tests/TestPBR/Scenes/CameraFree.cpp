#include "CameraFree.hpp"

#include <Maths/Maths.hpp>
#include <Scenes/Scenes.hpp>
#include <Inputs/AxisButton.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/ButtonJoystick.hpp>
#include <Inputs/AxisJoystick.hpp>

namespace test
{
const float WALK_SPEED = 3.0f;
const float RUN_SPEED = 7.0f;
const Vector3f DAMP(20.0f, 20.0f, 20.0f);

static const Vector2f SENSITIVITY_JOYSTICK(-0.06f);
static const Vector2f SENSITIVITY_MOUSE(0.15f);

CameraFree::CameraFree() :
	m_sensitivity{1.0f},
	m_joystickVertical{0, 3},
	m_joystickHorizontal{0, 2},
	m_inputForward{std::make_unique<AxisButton>(std::make_unique<ButtonKeyboard>(Key::W), std::make_unique<ButtonKeyboard>(Key::S)),
		std::make_unique<AxisJoystick>(0, 1)},
	m_inputStrafe{std::make_unique<AxisButton>(std::make_unique<ButtonKeyboard>(Key::A), std::make_unique<ButtonKeyboard>(Key::D)),
		std::make_unique<AxisJoystick>(0, 0)},
	m_inputVertical{std::make_unique<ButtonKeyboard>(Key::Space), std::make_unique<ButtonKeyboard>(Key::ControlLeft)},
	m_inputSprint{std::make_unique<ButtonKeyboard>(Key::ShiftLeft), std::make_unique<ButtonJoystick>(0, 1)}
{
	m_nearPlane = 0.1f;
	m_farPlane = 4098.0f;
	m_fieldOfView = Maths::Radians(70.0f);
}

void CameraFree::Start()
{
}

void CameraFree::Update()
{
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	if (!Scenes::Get()->IsPaused())
	{
		Vector3f positionDelta;

		if (!Scenes::Get()->IsPaused())
		{
			positionDelta.m_x = m_inputStrafe.GetAmount();
			positionDelta.m_y = m_inputVertical.GetAmount();
			positionDelta.m_z = m_inputForward.GetAmount();
		}

		positionDelta *= m_inputSprint.IsDown() ? -RUN_SPEED : -WALK_SPEED;
		m_velocity = m_velocity.SmoothDamp(positionDelta, delta * DAMP);

		Vector2f rotationDelta = Mouse::Get()->GetPositionDelta() * Mouse::Get()->IsCursorHidden() * SENSITIVITY_MOUSE;

		if (m_joystickVertical.IsConnected())
		{
			rotationDelta += Vector2f{m_joystickHorizontal.GetAmount(), m_joystickVertical.GetAmount()} * SENSITIVITY_JOYSTICK;
		}

		m_rotation.m_y += rotationDelta.m_x * m_sensitivity;
		m_rotation.m_x += rotationDelta.m_y * m_sensitivity;
		m_rotation.m_x = std::clamp(m_rotation.m_x, Maths::Radians(90.0f), Maths::Radians(270.0f));

		m_position.m_x += -(m_velocity.m_z * std::sin(m_rotation.m_y) + m_velocity.m_x * std::cos(m_rotation.m_y)) * delta;
		m_position.m_y += m_velocity.m_y * delta;
		m_position.m_z += (m_velocity.m_z * std::cos(m_rotation.m_y) - m_velocity.m_x * std::sin(m_rotation.m_y)) * delta;
	}

	m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation);
	m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView(), Window::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

	m_viewFrustum.Update(m_viewMatrix, m_projectionMatrix);
	m_viewRay.Update(m_position, {0.5f, 0.5f}, m_viewMatrix, m_projectionMatrix);
}
}
