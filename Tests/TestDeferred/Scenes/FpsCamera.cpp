#include "FpsCamera.hpp"

#include <Scenes/Scenes.hpp>
#include <Inputs/Mouse.hpp>
#include <Maths/Maths.hpp>
#include "FpsPlayer.hpp"

namespace test
{
	const float FpsCamera::NEAR_PLANE = 0.1f;
	const float FpsCamera::FAR_PLANE = 10000.0f;
	const float FpsCamera::FIELD_OF_VIEW = 60.0f;

	const float FpsCamera::ROTATE_AGILITY = 32.0f;
	const float FpsCamera::PITCH_AGILITY = 32.0f;

	const float FpsCamera::INFLUENCE_OF_JOYSTICK_DY = 4.5f;
	const float FpsCamera::INFLUENCE_OF_JOYSTICK_DX = 4.5f;

	const float FpsCamera::INFLUENCE_OF_MOUSE_DY = 10000.0f;
	const float FpsCamera::INFLUENCE_OF_MOUSE_DX = 10000.0f;

	const float FpsCamera::MAX_HORIZONTAL_CHANGE = 30.0f;
	const float FpsCamera::MAX_VERTICAL_CHANGE = 30.0f;

	const float FpsCamera::MAX_ANGLE_OF_ELEVATION = 85.0f;
	const float FpsCamera::MIN_ANGLE_OF_ELEVATION = -85.0f;

	FpsCamera::FpsCamera() :
		m_position(Vector3()),
		m_velocity(Vector3()),
		m_rotation(Vector3()),
		m_viewMatrix(Matrix4()),
		m_projectionMatrix(Matrix4()),
		m_viewFrustum(Frustum()),
		m_viewRay(Ray(false, Vector2(0.5f, 0.5f))),
		m_angleOfElevation(25.0f),
		m_angleAroundPlayer(0.0f),
		m_targetPosition(Vector3()),
		m_targetRotation(Vector3()),
		m_targetElevation(m_angleOfElevation),
		m_targetRotationAngle(m_angleAroundPlayer),
		m_sensitivity(0.9f),
		m_reangleButton(WSI_MOUSE_BUTTON_LEFT),
		m_joystickVertical(AxisJoystick(WSI_JOYSTICK_1, {3}, true)),
		m_joystickHorizontal(AxisJoystick(WSI_JOYSTICK_1, {2})),
		m_paused(false)
	{
	}

	FpsCamera::~FpsCamera()
	{
	}

	void FpsCamera::Update()
	{
		float delta = std::min(1.0f / 60.0f, Engine::Get()->GetDelta());

		const bool newPaused = Scenes::Get()->IsGamePaused();

		if (m_paused != newPaused)
		{
			Mouse::Get()->SetCursorHidden(!newPaused);
		}

		m_paused = newPaused;

		CalculateHorizontalAngle();
		CalculateVerticalAngle();

		auto player = Scenes::Get()->GetStructure()->GetComponent<FpsPlayer>();

		if (player != nullptr)
		{
			auto playerRotation = player->GetGameObject()->GetTransform().GetRotation();
			auto playerPosition = player->GetGameObject()->GetTransform().GetPosition();

			m_velocity = (playerPosition - m_targetPosition) / delta;
			m_targetPosition = playerPosition;
			m_targetRotation = playerRotation;
		}

		UpdateHorizontalAngle(delta);
		UpdatePitchAngle(delta);
		UpdatePosition();

		m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation);
		m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFov(), Display::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

		m_viewFrustum.Update(m_viewMatrix, m_projectionMatrix);
		m_viewRay.Update(m_position, Vector2(Mouse::Get()->GetPositionX(), Mouse::Get()->GetPositionY()), m_viewMatrix, m_projectionMatrix);
	}

	void FpsCamera::CalculateHorizontalAngle()
	{
		float angleChange = 0.0f;

		if (!m_paused)
		{
			if (Maths::Deadband(0.05f, m_joystickHorizontal.GetAmount()) != 0.0f)
			{
				angleChange = m_joystickHorizontal.GetAmount() * INFLUENCE_OF_JOYSTICK_DX * m_sensitivity;
			}
			else if (Mouse::Get()->IsCursorDisabled() || Mouse::Get()->GetButton(m_reangleButton))
			{
				angleChange = Engine::Get()->GetDelta() * -Mouse::Get()->GetDeltaX() * INFLUENCE_OF_MOUSE_DX * m_sensitivity;
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

	void FpsCamera::CalculateVerticalAngle()
	{
		float angleChange = 0.0f;

		if (!m_paused)
		{
			if (Maths::Deadband(0.05f, m_joystickVertical.GetAmount()) != 0.0f)
			{
				angleChange = m_joystickVertical.GetAmount() * INFLUENCE_OF_JOYSTICK_DY * m_sensitivity;
			}
			else if (Mouse::Get()->IsCursorDisabled() || Mouse::Get()->GetButton(m_reangleButton))
			{
				angleChange = Engine::Get()->GetDelta() * Mouse::Get()->GetDeltaY() * INFLUENCE_OF_MOUSE_DY * m_sensitivity;
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

	void FpsCamera::UpdateHorizontalAngle(const float &delta)
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

	void FpsCamera::UpdatePitchAngle(const float &delta)
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

	void FpsCamera::UpdatePosition()
	{
		m_position = m_targetPosition;
		m_rotation.m_x = m_angleOfElevation - m_targetRotation.m_z;
		m_rotation.m_y = m_angleAroundPlayer + m_targetRotation.m_y + 180.0f;
		m_rotation.m_z = 0.0f;
	}

	void FpsCamera::ReflectView(const float &height)
	{
		m_position.m_y -= 2.0f * (m_position.m_y - height);
		m_rotation.m_x = -m_rotation.m_x;
		m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation);
	}
}
