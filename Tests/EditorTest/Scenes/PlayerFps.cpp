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
	m_inputForward({ std::make_unique<AxisButton>(ButtonCompound::Create<ButtonKeyboard>(false, Key::S, Key::Down), ButtonCompound::Create<ButtonKeyboard>(false, Key::W, Key::Up)),
		std::make_unique<AxisJoystick>(0, 1, true) }),
	m_inputStrafe({ std::make_unique<AxisButton>(ButtonCompound::Create<ButtonKeyboard>(false, Key::D, Key::Right), ButtonCompound::Create<ButtonKeyboard>(false, Key::A, Key::Left)),
			std::make_unique<AxisJoystick>(0, 0, true) }),
	m_inputSprint({ ButtonCompound::Create<ButtonKeyboard>(false, Key::ShiftLeft, Key::ShiftRight), std::make_unique<ButtonJoystick>(0, 1) }),
	m_inputJump({ std::make_unique<ButtonKeyboard>(Key::Space), std::make_unique<ButtonJoystick>(0, 1) }),
	m_inputCrouch({ ButtonCompound::Create<ButtonKeyboard>(false, Key::ControlLeft, Key::ControlRight), std::make_unique<ButtonJoystick>(0, 1) }),
	m_toggleNoclip({ std::make_unique<ButtonKeyboard>(Key::N), })
{
}

void PlayerFps::Start()
{
	//auto collisionObject = GetParent()->GetComponent<CollisionObject>();
	//collisionObject->GetCollisionEvents().Subscribe([&](CollisionObject *other){ Log::Out("Player collided with '%s'\n", other->GetParent()->GetName());});
	//collisionObject->GetSeparationEvents().Subscribe([&](CollisionObject *other){ Log::Out("Player seperated with '%s'\n", other->GetParent()->GetName());});
}

void PlayerFps::Update()
{
	auto character = GetParent()->GetComponent<KinematicCharacter>();

	if (character == nullptr || !character->IsShapeCreated())
	{
		return;
	}

	Vector3f direction;

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
				character->Jump(Vector3f(0.0f, JUMP_SPEED, 0.0f));
			}
		}

		if (m_toggleNoclip.WasDown())
		{
			m_noclipEnabled = !m_noclipEnabled;

			if (m_noclipEnabled)
			{
				character->SetGravity(Vector3f());
				character->SetLinearVelocity(Vector3f());
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

	transform.SetRotation(Vector3f(0.0f, cameraRotation.m_y, 0.0f));

	auto walkDirection = direction;
	walkDirection.m_x = -(direction.m_z * std::sin(cameraRotation.m_y) + direction.m_x * std::cos(cameraRotation.m_y));
	walkDirection.m_z = direction.m_z * std::cos(cameraRotation.m_y) - direction.m_x * std::sin(cameraRotation.m_y);

	//walkDirection = walkDirection.Normalize();
	walkDirection *= m_inputSprint.IsDown() ? RUN_SPEED : m_inputCrouch.IsDown() ? CROUCH_SPEED : WALK_SPEED;
	walkDirection *= m_noclipEnabled ? NOCLIP_SPEED : 1.0f;
	character->SetWalkDirection(0.02f * walkDirection);
}

const Metadata &operator>>(const Metadata &metadata, PlayerFps &player)
{
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const PlayerFps &player)
{
	return metadata;
}
}
