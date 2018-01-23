#include "FpsPlayer.hpp"

#include <Camera/Camera.hpp>
#include <Inputs/AxisCompound.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/AxisButton.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Inputs/ButtonJoystick.hpp>
#include <Uis/Uis.hpp>
#include <Terrains/Terrains.hpp>

namespace Demo
{
	const float FpsPlayer::WALK_SPEED = 3.1f;
	const float FpsPlayer::RUN_SPEED = 5.7f;
	const float FpsPlayer::CROUCH_SPEED = 1.2f;
	const float FpsPlayer::JUMP_SPEED = 6.5f;
	const float FpsPlayer::NOCLIP_SPEED = 15.0f;

	FpsPlayer::FpsPlayer() :
		IPlayer(),
		m_position(new Vector3()),
		m_rotation(new Vector3()),
		m_currentSpeed(0.0f),
		m_currentStrafeSpeed(0.0f),
		m_currentUpwardSpeed(0.0f),
		m_jumping(false),
		m_noclipEnabled(false),
		m_inputForward(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({GLFW_KEY_S, GLFW_KEY_DOWN}),
				new ButtonKeyboard({GLFW_KEY_W, GLFW_KEY_UP})
			),
			new AxisJoystick(0, {1})
		})),
		m_inputStrafe(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({GLFW_KEY_D, GLFW_KEY_RIGHT}),
				new ButtonKeyboard({GLFW_KEY_A, GLFW_KEY_LEFT})
			),
			new AxisJoystick(0, {0})
		})),
		m_inputSprint(new ButtonCompound({
			new ButtonKeyboard({GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT}),
			new ButtonJoystick(0, {1})
		})),
		m_inputJump(new ButtonCompound({
			new ButtonKeyboard({GLFW_KEY_SPACE}),
			new ButtonJoystick(0, {1})
		})),
		m_inputCrouch(new ButtonCompound({
			new ButtonKeyboard({GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL}),
			new ButtonJoystick(0, {1})
		})),
		m_toggleNoclip(new ButtonCompound({
			new ButtonKeyboard({GLFW_KEY_N}),
		})),
		m_amountMove(new Vector3()),
		m_amountRotate(new Vector3()),
		m_paused(false)
	{
	}

	FpsPlayer::~FpsPlayer()
	{
		delete m_position;
		delete m_rotation;

		delete m_inputForward;
		delete m_inputStrafe;

		delete m_inputSprint;
		delete m_inputCrouch;
		delete m_inputJump;
		delete m_toggleNoclip;

		delete m_amountMove;
		delete m_amountRotate;
	}

	void FpsPlayer::Update()
	{
		// Gets the delta and limits the lowest UPS to 20 (any less and the game is unplayable).
		const float delta = Maths::Min(Engine::Get()->GetDelta(), 1.0f / 20.0f);

		if (Uis::Get() != nullptr && Uis::Get()->GetManager() != nullptr)
		{
			m_paused = Uis::Get()->GetManager()->IsGamePaused();
		}

		if (!m_paused)
		{
			const bool sprintDown = m_inputSprint->IsDown();
			const bool crouchDown = m_inputCrouch->IsDown();
			m_currentSpeed = (sprintDown ? RUN_SPEED : crouchDown ? CROUCH_SPEED : WALK_SPEED) * m_inputForward->GetAmount();
			m_currentStrafeSpeed = (sprintDown ? RUN_SPEED : crouchDown ? CROUCH_SPEED : WALK_SPEED) * m_inputStrafe->GetAmount();

			if (m_noclipEnabled)
			{
				if (m_inputJump->IsDown())
				{
					m_currentUpwardSpeed = sprintDown ? RUN_SPEED : WALK_SPEED;
				}
				else if (m_inputCrouch->IsDown())
				{
					m_currentUpwardSpeed = sprintDown ? -RUN_SPEED : -WALK_SPEED;
				}
				else
				{
					m_currentUpwardSpeed = 0.0f;
				}

				m_currentSpeed *= NOCLIP_SPEED;
				m_currentStrafeSpeed *= NOCLIP_SPEED;
				m_currentUpwardSpeed *= NOCLIP_SPEED;
			}
			else
			{
				if (m_inputJump->WasDown() && !m_jumping)
				{
					m_currentUpwardSpeed = crouchDown ? 3.0f : JUMP_SPEED;
					m_jumping = true;
				}
			}

			if (m_toggleNoclip->WasDown())
			{
				m_currentSpeed = 0.0f;
				m_currentStrafeSpeed = 0.0f;
				m_currentUpwardSpeed = 0.0f;
				m_jumping = false;
				m_noclipEnabled = !m_noclipEnabled;
				printf("Player Noclip: %s\n", m_noclipEnabled ? "true" : "false");
			}
		}
		else
		{
			m_currentSpeed = 0.0f;
			m_currentStrafeSpeed = 0.0f;
			//	m_currentUpwardSpeed = 0.0f;
		}

		const float terrainHeight = Terrains::Get()->GetHeight(m_position->m_x, m_position->m_z) + 1.74f;
		// const float terrainSlope = Terrains::Get()->GetSlope(m_position->m_x, m_position->m_z);

		/*if (terrainSlope - 0.3 > 0)
		{
			m_currentSpeed *= terrainSlope - 0.5;
		}*/

		// Calculates the deltas to the moved distance, and rotations.
		const float theta = Maths::Radians(Camera::Get()->GetCamera()->GetRotation()->m_y);
		const float dx = -(m_currentSpeed * std::sin(theta) + m_currentStrafeSpeed * std::cos(theta)) * delta;
		const float dy = m_currentUpwardSpeed * delta;
		const float dz = -(m_currentSpeed * std::cos(theta) - m_currentStrafeSpeed * std::sin(theta)) * delta;

		Vector3::Add(*m_position, *m_amountMove->Set(dx, dy, dz), m_position);
		Vector3::Add(*m_rotation, *m_amountRotate->Set(0.0f, 0.0f, 0.0f), m_rotation);

		if (!m_noclipEnabled)
		{
			m_currentUpwardSpeed -= 24.06f * delta;

			if (m_position->m_y < terrainHeight)
			{
				m_position->m_y = terrainHeight;
				m_currentUpwardSpeed = 0.0f;
				m_jumping = false;
			}
		}
	}
}
