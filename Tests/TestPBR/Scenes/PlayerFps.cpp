#include "PlayerFps.hpp"

#include <Uis/Uis.hpp>
#include <Scenes/Scenes.hpp>
#include <Inputs/AxisButton.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/AxisJoystick.hpp>

namespace test
{
	const float WALK_SPEED = 3.1f;
	const float RUN_SPEED = 5.7f;
	const float CROUCH_SPEED = 1.2f;
	const float JUMP_SPEED = 6.5f;
	const float CROUCH_JUMP_SPEED = 3.0f;
	const float NOCLIP_SPEED = 4.0f;
	const float GRAVITY = -9.81f;
	const Vector3 DAMP_NOCLIP = Vector3(10.0f, 10.0f, 10.0f);
	const Vector3 DAMP_NORMAL = Vector3(20.0f, 1.0f, 20.0f);

	PlayerFps::PlayerFps() :
		m_velocity(Vector3()),
		m_jumping(false),
		m_noclipEnabled(true),
		m_inputForward(AxisCompound({
		    new AxisButton(
		        ButtonCompound::Create<ButtonKeyboard>(KEY_S, KEY_DOWN),
		        ButtonCompound::Create<ButtonKeyboard>(KEY_W, KEY_UP)
		    ),
		    new AxisJoystick(JOYSTICK_1, 1, true)
		})),
		m_inputStrafe(AxisCompound({
		    new AxisButton(
		        ButtonCompound::Create<ButtonKeyboard>(KEY_D, KEY_RIGHT),
		        ButtonCompound::Create<ButtonKeyboard>(KEY_A, KEY_LEFT)
		    ),
		    new AxisJoystick(JOYSTICK_1, 0, true)
		})),
		m_inputSprint(ButtonCompound({
		    ButtonCompound::Create<ButtonKeyboard>(KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT),
		    new ButtonJoystick(JOYSTICK_1, 1)
		})),
		m_inputJump(ButtonCompound({
		    new ButtonKeyboard(KEY_SPACE),
		    new ButtonJoystick(JOYSTICK_1, 1)
		})),
		m_inputCrouch(ButtonCompound({
		    ButtonCompound::Create<ButtonKeyboard>(KEY_LEFT_CONTROL, KEY_RIGHT_CONTROL),
		    new ButtonJoystick(JOYSTICK_1, 1)
		})),
		m_toggleNoclip(ButtonCompound({
		    ButtonCompound::Create<ButtonKeyboard>(KEY_N)
		})),
		m_amountMove(Vector3()),
		m_amountRotate(Vector3())
	{
	}

	void PlayerFps::Start()
	{
		Mouse::Get()->SetCursorHidden(true);
	}

	void PlayerFps::Update()
	{
		// Gets the delta and limits the lowest UPS to 20 (any less and the game is unplayable).
		float delta = std::min(1.0f / 20.0f, Engine::Get()->GetDelta().AsSeconds());

		Vector3 targetVelocity = Vector3(0.0f, m_noclipEnabled ? 0.0f : GRAVITY, 0.0f);

		if (!Scenes::Get()->IsPaused())
		{
			bool sprintDown = m_inputSprint.IsDown();
			bool crouchDown = m_inputCrouch.IsDown();

			targetVelocity.m_z += (sprintDown ? RUN_SPEED : crouchDown ? CROUCH_SPEED : WALK_SPEED) * m_inputForward.GetAmount();
			targetVelocity.m_x += (sprintDown ? RUN_SPEED : crouchDown ? CROUCH_SPEED : WALK_SPEED) * m_inputStrafe.GetAmount();

			if (m_noclipEnabled)
			{
				if (m_inputJump.IsDown())
				{
					targetVelocity.m_y += sprintDown ? RUN_SPEED : WALK_SPEED;
				}
				else if (m_inputCrouch.IsDown())
				{
					targetVelocity.m_y += sprintDown ? -RUN_SPEED : -WALK_SPEED;
				}

				targetVelocity *= NOCLIP_SPEED;
			}
			else
			{
				if (m_inputJump.WasDown() && !m_jumping)
				{
					targetVelocity.m_y += crouchDown ? CROUCH_JUMP_SPEED : JUMP_SPEED;
					m_velocity.m_y += targetVelocity.m_y;
					m_jumping = true;
				}
			}

			if (m_toggleNoclip.WasDown())
			{
				targetVelocity *= 0.0f;
				m_jumping = false;
				m_noclipEnabled = !m_noclipEnabled;
				Log::Out("Player Noclip: %s\n", m_noclipEnabled ? "true" : "false");
			}
		}

		m_velocity = m_velocity.SmoothDamp(targetVelocity, delta * (m_noclipEnabled ? DAMP_NOCLIP : DAMP_NORMAL));

		auto cameraRotation = Scenes::Get()->GetCamera()->GetRotation();
		Vector3 newPosition = GetParent()->GetLocalTransform().GetPosition();
		Vector3 newRotation = GetParent()->GetLocalTransform().GetRotation();

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

		GetParent()->GetLocalTransform().SetPosition(newPosition);
		GetParent()->GetLocalTransform().SetRotation(newRotation);
	}

	void PlayerFps::Decode(const Metadata &metadata)
	{
	}

	void PlayerFps::Encode(Metadata &metadata) const
	{
	}
}
