#include "FpsPlayer.hpp"

#include <Inputs/AxisCompound.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/AxisButton.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Inputs/ButtonJoystick.hpp>
#include <Uis/Uis.hpp>
#include <Scenes/Scenes.hpp>

namespace Demo
{
	const float WALK_SPEED = 3.1f;
	const float RUN_SPEED = 5.7f;
	const float CROUCH_SPEED = 1.2f;
	const float JUMP_SPEED = 6.5f;
	const float CROUCH_JUMP_SPEED = 3.0f;
	const float NOCLIP_SPEED = 15.0f;
	const float GRAVITY = -9.81f;
	const Vector3 DAMP_NOCLIP = Vector3(10.0f, 10.0f, 10.0f);
	const Vector3 DAMP_NORMAL = Vector3(20.0f, 1.0f, 20.0f);

	FpsPlayer::FpsPlayer() :
		Behaviour(),
		m_velocity(new Vector3()),
		m_jumping(false),
		m_noclipEnabled(true),
		m_inputForward(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({GLFW_KEY_S, GLFW_KEY_DOWN}),
				new ButtonKeyboard({GLFW_KEY_W, GLFW_KEY_UP})
			),
			new AxisJoystick(0, {1}, true)
		})),
		m_inputStrafe(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({GLFW_KEY_D, GLFW_KEY_RIGHT}),
				new ButtonKeyboard({GLFW_KEY_A, GLFW_KEY_LEFT})
			),
			new AxisJoystick(0, {0}, true)
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
		m_amountRotate(new Vector3())
	{
	}

	FpsPlayer::~FpsPlayer()
	{
		delete m_velocity;

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
		float delta = std::min(Engine::Get()->GetDelta(), 1.0f / 20.0f);

		Vector3 targetVelocity = Vector3(0.0f, m_noclipEnabled ? 0.0f : GRAVITY, 0.0f);

		if (!Scenes::Get()->IsGamePaused())
		{
			bool sprintDown = m_inputSprint->IsDown();
			bool crouchDown = m_inputCrouch->IsDown();

			targetVelocity.m_z += (sprintDown ? RUN_SPEED : crouchDown ? CROUCH_SPEED : WALK_SPEED) * m_inputForward->GetAmount();
			targetVelocity.m_x += (sprintDown ? RUN_SPEED : crouchDown ? CROUCH_SPEED : WALK_SPEED) * m_inputStrafe->GetAmount();

			if (m_noclipEnabled)
			{
				if (m_inputJump->IsDown())
				{
					targetVelocity.m_y += sprintDown ? RUN_SPEED : WALK_SPEED;
				}
				else if (m_inputCrouch->IsDown())
				{
					targetVelocity.m_y += sprintDown ? -RUN_SPEED : -WALK_SPEED;
				}

				targetVelocity *= NOCLIP_SPEED;
			}
			else
			{
				if (m_inputJump->WasDown() && !m_jumping)
				{
					targetVelocity.m_y += crouchDown ? CROUCH_JUMP_SPEED : JUMP_SPEED;
					m_velocity->m_y += targetVelocity.m_y;
					m_jumping = true;
				}
			}

			if (m_toggleNoclip->WasDown())
			{
				targetVelocity *= 0.0f;
				m_jumping = false;
				m_noclipEnabled = !m_noclipEnabled;
				printf("Player Noclip: %s\n", m_noclipEnabled ? "true" : "false");
			}
		}

		*m_velocity = m_velocity->SmoothDamp(targetVelocity, delta * (m_noclipEnabled ? DAMP_NOCLIP : DAMP_NORMAL));

		auto cameraRotation = Scenes::Get()->GetCamera()->GetRotation();
		auto position = GetGameObject()->GetTransform()->GetPosition();
		auto rotation = GetGameObject()->GetTransform()->GetRotation();

		float groundHeight = 0.0f;

		// Calculates the deltas to the moved distance, and rotation.
		float theta = Maths::Radians(cameraRotation->m_y);
		float dx = -(m_velocity->m_z * std::sin(theta) + m_velocity->m_x * std::cos(theta)) * delta;
		float dy = m_velocity->m_y * delta;
		float dz = -(m_velocity->m_z * std::cos(theta) - m_velocity->m_x * std::sin(theta)) * delta;

		*m_amountMove = Vector3(dx, dy, dz);
		*m_amountRotate = Vector3(0.0f, 0.0f, 0.0f);

		*position = *position + *m_amountMove;
		*rotation = *rotation + *m_amountRotate;

		if (!m_noclipEnabled && position->m_y <= groundHeight)
		{
			m_velocity->m_y = 0.0f;
			m_jumping = false;
			position->m_y = groundHeight;
		}
	}
}
