#include "FpsPlayer.hpp"

#include <Inputs/AxisCompound.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/AxisButton.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Inputs/ButtonJoystick.hpp>
#include <Uis/Uis.hpp>
#include <Scenes/Scenes.hpp>

namespace test
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
		IBehaviour(),
		m_velocity(Vector3()),
		m_jumping(false),
		m_noclipEnabled(true),
		m_inputForward(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({WSI_KEY_S, WSI_KEY_DOWN}),
				new ButtonKeyboard({WSI_KEY_W, WSI_KEY_UP})
			),
			new AxisJoystick(WSI_JOYSTICK_1, {1}, true)
		})),
		m_inputStrafe(new AxisCompound({
			new AxisButton(
				new ButtonKeyboard({WSI_KEY_D, WSI_KEY_RIGHT}),
				new ButtonKeyboard({WSI_KEY_A, WSI_KEY_LEFT})
			),
			new AxisJoystick(WSI_JOYSTICK_1, {0}, true)
		})),
		m_inputSprint(new ButtonCompound({
			new ButtonKeyboard({WSI_KEY_LEFT_SHIFT, WSI_KEY_RIGHT_SHIFT}),
			new ButtonJoystick(WSI_JOYSTICK_1, {1})
		})),
		m_inputJump(new ButtonCompound({
			new ButtonKeyboard({WSI_KEY_SPACE}),
			new ButtonJoystick(WSI_JOYSTICK_1, {1})
		})),
		m_inputCrouch(new ButtonCompound({
			new ButtonKeyboard({WSI_KEY_LEFT_CONTROL, WSI_KEY_RIGHT_CONTROL}),
			new ButtonJoystick(WSI_JOYSTICK_1, {1})
		})),
		m_toggleNoclip(new ButtonCompound({
			new ButtonKeyboard({WSI_KEY_N}),
		})),
		m_amountMove(Vector3()),
		m_amountRotate(Vector3())
	{
	}

	FpsPlayer::~FpsPlayer()
	{
		delete m_inputForward;
		delete m_inputStrafe;

		delete m_inputSprint;
		delete m_inputCrouch;
		delete m_inputJump;
		delete m_toggleNoclip;
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
					m_velocity.m_y += targetVelocity.m_y;
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

		m_velocity = m_velocity.SmoothDamp(targetVelocity, delta * (m_noclipEnabled ? DAMP_NOCLIP : DAMP_NORMAL));

		auto cameraRotation = Scenes::Get()->GetCamera()->GetRotation();
		Vector3 newPosition = GetGameObject()->GetTransform().GetPosition();
		Vector3 newRotation = GetGameObject()->GetTransform().GetRotation();

		float groundHeight = 0.0f;

		// Calculates the deltas to the moved distance, and rotation.
		float theta = Maths::Radians(cameraRotation.m_y);
		float dx = -(m_velocity.m_z * std::sin(theta) + m_velocity.m_x * std::cos(theta)) * delta;
		float dy = m_velocity.m_y * delta;
		float dz = -(m_velocity.m_z * std::cos(theta) - m_velocity.m_x * std::sin(theta)) * delta;

		m_amountMove = Vector3(dx, dy, dz);
		m_amountRotate = Vector3(0.0f, 0.0f, 0.0f);

		newPosition += m_amountMove;
		newRotation += m_amountRotate;

		if (!m_noclipEnabled && newPosition.m_y <= groundHeight)
		{
			m_velocity.m_y = 0.0f;
			m_jumping = false;
			newPosition.m_y = groundHeight;
		}

		GetGameObject()->GetTransform().SetPosition(newPosition);
		GetGameObject()->GetTransform().SetRotation(newRotation);
	}
}
