#include "PlayerFps.hpp"

#include <Uis/Uis.hpp>
#include <Scenes/Scenes.hpp>
#include <Inputs/AxisButton.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Physics/KinematicCharacter.hpp>
#include <Inputs/ButtonJoystick.hpp>

namespace test
{
const float WALK_SPEED = 3.1f;
const float RUN_SPEED = 5.7f;
const float CROUCH_SPEED = 1.2f;
const float JUMP_SPEED = 4.1f;
const float NOCLIP_SPEED = 3.0f;

PlayerFps::PlayerFps() :
	m_noclipEnabled(false),
	m_inputForward(AxisCompound({ new AxisButton(ButtonCompound::Create<ButtonKeyboard>(false, Key::S, Key::Down), ButtonCompound::Create<ButtonKeyboard>(false, Key::W, Key::Up)),
		new AxisJoystick(0, 1, true) })),
	m_inputStrafe(AxisCompound(
		{ new AxisButton(ButtonCompound::Create<ButtonKeyboard>(false, Key::D, Key::Right), ButtonCompound::Create<ButtonKeyboard>(false, Key::A, Key::Left)),
			new AxisJoystick(0, 0, true) })),
	m_inputSprint(ButtonCompound({ ButtonCompound::Create<ButtonKeyboard>(false, Key::ShiftLeft, Key::ShiftRight), new ButtonJoystick(0, 1) })),
	m_inputJump(ButtonCompound({ new ButtonKeyboard(Key::Space), new ButtonJoystick(0, 1) })),
	m_inputCrouch(ButtonCompound({ ButtonCompound::Create<ButtonKeyboard>(false, Key::ControlLeft, Key::ControlRight), new ButtonJoystick(0, 1) })),
	m_toggleNoclip(ButtonCompound({ new ButtonKeyboard(Key::N), }))
{
}

void PlayerFps::Start()
{
	//auto collisionObject = GetParent()->GetComponent<CollisionObject>();
	//collisionObject->GetCollisionEvents().Subscribe([&](CollisionObject *other){ Log::Out("Player collided with '%s'\n", other->GetParent()->GetName().c_str());});
	//collisionObject->GetSeparationEvents().Subscribe([&](CollisionObject *other){ Log::Out("Player seperated with '%s'\n", other->GetParent()->GetName().c_str());});
}

void PlayerFps::Update()
{
	auto character = GetParent()->GetComponent<KinematicCharacter>();

	if (character == nullptr || !character->IsShapeCreated())
	{
		return;
	}

	Vector3 direction = Vector3();

	if (!Scenes::Get()->IsPaused())
	{
		direction.m_x = m_inputStrafe.GetAmount();
		direction.m_z = m_inputForward.GetAmount();

		if (m_noclipEnabled)
		{
			if (m_inputJump.IsDown())
			{
				direction.m_y = 1.0f;
			}
			else if (m_inputCrouch.IsDown())
			{
				direction.m_y = -1.0f;
			}
		}
		else
		{
			if (m_inputJump.WasDown() && character->IsOnGround())
			{
				character->Jump(Vector3(0.0f, JUMP_SPEED, 0.0f));
			}
		}

		if (m_toggleNoclip.WasDown())
		{
			m_noclipEnabled = !m_noclipEnabled;

			if (m_noclipEnabled)
			{
				character->SetGravity(Vector3::Zero);
			}
			else
			{
				character->SetGravity(Scenes::Get()->GetPhysics()->GetGravity());
			}

			Log::Out("Player Noclip: %s\n", m_noclipEnabled ? "true" : "false");
		}
	}

	auto &transform = GetParent()->GetLocalTransform();
	auto cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

	transform.SetRotation(Vector3(0.0f, cameraRotation.m_y, 0.0f));

	float theta = cameraRotation.m_y * Maths::DegToRad;
	Vector3 walkDirection = direction;
	walkDirection.m_x = -(direction.m_z * std::sin(theta) + direction.m_x * std::cos(theta));
	walkDirection.m_z = direction.m_z * std::cos(theta) - direction.m_x * std::sin(theta);

	//walkDirection = walkDirection.Normalize();
	walkDirection *= m_inputSprint.IsDown() ? RUN_SPEED : m_inputCrouch.IsDown() ? CROUCH_SPEED : WALK_SPEED;
	walkDirection *= m_noclipEnabled ? NOCLIP_SPEED : 1.0f;
	character->SetWalkDirection(0.02f * walkDirection);
}

void PlayerFps::Decode(const Metadata &metadata)
{
}

void PlayerFps::Encode(Metadata &metadata) const
{
}
}
