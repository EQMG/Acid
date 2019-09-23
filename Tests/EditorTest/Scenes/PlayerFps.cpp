#include "PlayerFps.hpp"

#include <Uis/Uis.hpp>
#include <Scenes/Scenes.hpp>
#include <Inputs/AxisButton.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Physics/KinematicCharacter.hpp>
#include <Inputs/ButtonJoystick.hpp>

namespace test {
bool PlayerFps::registered = Register("playerFps");

const float WALK_SPEED = 3.1f;
const float RUN_SPEED = 5.7f;
const float CROUCH_SPEED = 1.2f;
const float JUMP_SPEED = 4.1f;
const float NOCLIP_SPEED = 3.0f;

PlayerFps::PlayerFps() :
	m_inputForward(std::make_unique<AxisButton>(std::make_unique<ButtonKeyboard>(Key::S), std::make_unique<ButtonKeyboard>(Key::W)),
		std::make_unique<AxisJoystick>(0, 1)),
	m_inputStrafe(std::make_unique<AxisButton>(std::make_unique<ButtonKeyboard>(Key::D), std::make_unique<ButtonKeyboard>(Key::A)),
		std::make_unique<AxisJoystick>(0, 0)),
	m_inputSprint(std::make_unique<ButtonKeyboard>(Key::ShiftLeft), std::make_unique<ButtonJoystick>(0, 1)),
	m_inputJump(std::make_unique<ButtonKeyboard>(Key::Space), std::make_unique<ButtonJoystick>(0, 2)),
	m_inputCrouch(std::make_unique<ButtonKeyboard>(Key::ControlLeft), std::make_unique<ButtonJoystick>(0, 3)),
	m_toggleNoclip(std::make_unique<ButtonKeyboard>(Key::N)) {
}

void PlayerFps::Start() {
}

void PlayerFps::Update() {
	auto character = GetEntity()->GetComponent<KinematicCharacter>();

	if (!character || !character->IsShapeCreated()) {
		return;
	}

	Vector3f direction;

	if (!Scenes::Get()->IsPaused()) {
		direction.m_x = m_inputStrafe.GetAmount();
		direction.m_z = m_inputForward.GetAmount();

		if (m_noclipEnabled) {
			if (m_inputJump.IsDown()) {
				direction.m_y = 1.0f;
			} else if (m_inputCrouch.IsDown()) {
				direction.m_y = -1.0f;
			}
		} else {
			if (m_inputJump.WasDown() && character->IsOnGround()) {
				character->Jump({0.0f, JUMP_SPEED, 0.0f});
			}
		}

		if (m_toggleNoclip.WasDown()) {
			m_noclipEnabled = !m_noclipEnabled;

			if (m_noclipEnabled) {
				character->SetGravity({});
				character->SetLinearVelocity({});
			} else {
				character->SetGravity(Scenes::Get()->GetPhysics()->GetGravity());
			}

			Log::Out("Player Noclip: ", m_noclipEnabled);
		}
	}

	auto cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

	if (auto transform = GetEntity()->GetComponent<Transform>(); transform) {
		transform->SetLocalRotation({0.0f, cameraRotation.m_y, 0.0f});
	}

	auto walkDirection = direction;
	walkDirection.m_x = -(direction.m_z * std::sin(cameraRotation.m_y) + direction.m_x * std::cos(cameraRotation.m_y));
	walkDirection.m_z = direction.m_z * std::cos(cameraRotation.m_y) - direction.m_x * std::sin(cameraRotation.m_y);

	//walkDirection = walkDirection.Normalize();
	walkDirection *= m_inputSprint.IsDown() ? RUN_SPEED : m_inputCrouch.IsDown() ? CROUCH_SPEED : WALK_SPEED;
	walkDirection *= m_noclipEnabled ? NOCLIP_SPEED : 1.0f;
	character->SetWalkDirection(0.02f * walkDirection);
}

const Node &operator>>(const Node &node, PlayerFps &player) {
	return node;
}

Node &operator<<(Node &node, const PlayerFps &player) {
	return node;
}
}
