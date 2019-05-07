#include "CameraFps.hpp"

#include <Scenes/Scenes.hpp>
#include <Devices/Mouse.hpp>
#include <Maths/Maths.hpp>
#include <Graphics/Graphics.hpp>
#include <Physics/CollisionObject.hpp>
#include "PlayerFps.hpp"

namespace test
{
static const float NEAR_PLANE = 0.1f;
static const float FAR_PLANE = 4098.0f;
static const float FIELD_OF_VIEW = 70.0f;

static const float ROTATE_AGILITY = 32.0f;
static const float PITCH_AGILITY = 32.0f;

static const float VIEW_HEIGHT = 1.8f;

static const float INFLUENCE_OF_JOYSTICK_DY = 4.5f;
static const float INFLUENCE_OF_JOYSTICK_DX = 4.5f;

static const float INFLUENCE_OF_MOUSE_DY = 10000.0f;
static const float INFLUENCE_OF_MOUSE_DX = 10000.0f;

static const float MAX_HORIZONTAL_CHANGE = 30.0f;
static const float MAX_VERTICAL_CHANGE = 30.0f;

static const float MAX_ANGLE_OF_ELEVATION = 270.0f;
static const float MIN_ANGLE_OF_ELEVATION = 90.0f;

CameraFps::CameraFps() :
	m_angleOfElevation(180.0f),
	m_angleAroundPlayer(0.0f),
	m_targetElevation(m_angleOfElevation),
	m_targetRotationAngle(m_angleAroundPlayer),
	m_sensitivity(0.6f),
	m_reangleButton(MouseButton::Left),
	m_joystickVertical(0, 3, true),
	m_joystickHorizontal(0, 2),
	m_paused(false)
{
	m_nearPlane = NEAR_PLANE;
	m_farPlane = FAR_PLANE;
	m_fieldOfView = FIELD_OF_VIEW;
}

void CameraFps::Start()
{
}

void CameraFps::Update()
{
	float delta = std::min(1.0f / 60.0f, Engine::Get()->GetDelta().AsSeconds());

	const bool newPaused = Scenes::Get()->IsPaused();

	if (m_paused != newPaused)
	{
		Mouse::Get()->SetCursorHidden(!newPaused);
	}

	m_paused = newPaused;

	CalculateHorizontalAngle();
	CalculateVerticalAngle();

	auto scenePlayer = Scenes::Get()->GetStructure()->GetComponent<PlayerFps>();

	if (scenePlayer != nullptr)
	{
		auto playerPosition = scenePlayer->GetParent()->GetWorldTransform().GetPosition();
		//auto playerRotation = scenePlayer->GetParent()->GetWorldTransform().GetRotation();

		m_velocity = (playerPosition - m_targetPosition) / delta;
		m_targetPosition = playerPosition + Vector3f(0.0f, VIEW_HEIGHT, 0.0f);
		//m_targetRotation = playerRotation;
	}

	UpdateHorizontalAngle(delta);
	UpdatePitchAngle(delta);
	UpdatePosition();

	m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation * Maths::DegToRad);
	m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView() * Maths::DegToRad, Window::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

	m_viewFrustum.Update(m_viewMatrix, m_projectionMatrix);
	m_viewRay.Update(m_position, Vector2f(0.5f, 0.5f), m_viewMatrix, m_projectionMatrix); // Mouse::Get()->GetPositionX(), Mouse::Get()->GetPositionY()

	//auto raytest = Scenes::Get()->GetPhysics()->Raytest(m_viewRay.GetOrigin(), m_viewRay.GetPointOnRay(20.0f));
	//Log::Out("%s: %f\n", raytest.HasHit() ? raytest.GetParent()->GetName().c_str() : "", raytest.GetPointWorld().Distance(m_viewRay.GetOrigin()));
}

void CameraFps::CalculateHorizontalAngle()
{
	float angleChange = 0.0f;

	if (!m_paused)
	{
		if (Maths::Deadband(0.05f, m_joystickHorizontal.GetAmount()) != 0.0f)
		{
			angleChange = m_joystickHorizontal.GetAmount() * INFLUENCE_OF_JOYSTICK_DX * m_sensitivity;
		}
		else if (Mouse::Get()->IsCursorHidden() || Mouse::Get()->GetButton(m_reangleButton) != InputAction::Release)
		{
			angleChange = -Mouse::Get()->GetDelta().m_x * INFLUENCE_OF_MOUSE_DX * m_sensitivity;
		}
	}

	if (angleChange > MAX_HORIZONTAL_CHANGE)
	{
		angleChange = MAX_HORIZONTAL_CHANGE;
	}
	else if (angleChange < -MAX_HORIZONTAL_CHANGE)
	{
		angleChange = -MAX_HORIZONTAL_CHANGE;
	}

	m_targetRotationAngle -= angleChange;

	if (m_targetRotationAngle >= 180.0f)
	{
		m_targetRotationAngle -= 360.0f;
	}
	else if (m_targetRotationAngle <= -180.0f)
	{
		m_targetRotationAngle += 360.0f;
	}
}

void CameraFps::CalculateVerticalAngle()
{
	float angleChange = 0.0f;

	if (!m_paused)
	{
		if (Maths::Deadband(0.05f, m_joystickVertical.GetAmount()) != 0.0f)
		{
			angleChange = m_joystickVertical.GetAmount() * INFLUENCE_OF_JOYSTICK_DY * m_sensitivity;
		}
		else if (Mouse::Get()->IsCursorHidden() || Mouse::Get()->GetButton(m_reangleButton) != InputAction::Release)
		{
			angleChange = -Mouse::Get()->GetDelta().m_y * INFLUENCE_OF_MOUSE_DY * m_sensitivity;
		}
	}

	if (angleChange > MAX_VERTICAL_CHANGE)
	{
		angleChange = MAX_VERTICAL_CHANGE;
	}
	else if (angleChange < -MAX_VERTICAL_CHANGE)
	{
		angleChange = -MAX_VERTICAL_CHANGE;
	}

	m_targetElevation -= angleChange;

	if (m_targetElevation >= MAX_ANGLE_OF_ELEVATION)
	{
		m_targetElevation = MAX_ANGLE_OF_ELEVATION;
	}
	else if (m_targetElevation <= MIN_ANGLE_OF_ELEVATION)
	{
		m_targetElevation = MIN_ANGLE_OF_ELEVATION;
	}
}

void CameraFps::UpdateHorizontalAngle(const float &delta)
{
	float offset = m_targetRotationAngle - m_angleAroundPlayer;

	if (std::fabs(offset) > 180.0f)
	{
		if (offset < 0.0f)
		{
			offset = m_targetRotationAngle + 360.0f - m_angleAroundPlayer;
		}
		else
		{
			offset = m_targetRotationAngle - 360.0f - m_angleAroundPlayer;
		}
	}

	m_angleAroundPlayer += offset * delta * ROTATE_AGILITY;

	if (m_angleAroundPlayer >= 180.0f)
	{
		m_angleAroundPlayer -= 360.0f;
	}
	else if (m_angleAroundPlayer <= -180.0f)
	{
		m_angleAroundPlayer += 360.0f;
	}
}

void CameraFps::UpdatePitchAngle(const float &delta)
{
	float offset = m_targetElevation - m_angleOfElevation;

	if (std::fabs(offset) > 180.0f)
	{
		if (offset < 0.0f)
		{
			offset = m_targetElevation + 360.0f - m_angleOfElevation;
		}
		else
		{
			offset = m_targetElevation - 360.0f - m_angleOfElevation;
		}
	}

	m_angleOfElevation += offset * delta * PITCH_AGILITY;

	if (m_angleOfElevation >= 180.0f)
	{
		m_angleOfElevation -= 360.0f;
	}
	else if (m_angleOfElevation <= -180.0f)
	{
		m_angleOfElevation += 360.0f;
	}
}

void CameraFps::UpdatePosition()
{
	m_position = m_targetPosition;
	m_rotation.m_x = m_angleOfElevation - m_targetRotation.m_z;
	m_rotation.m_y = m_angleAroundPlayer + m_targetRotation.m_y;
	m_rotation.m_z = 0.0f;
}
}
