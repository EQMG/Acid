#include "FpsPlayer.hpp"

#include <Uis/Uis.hpp>
#include <Scenes/Scenes.hpp>
#include <Inputs/Inputs.hpp>
#include <Physics/KinematicCharacter.hpp>
#include <Physics/Physics.hpp>

namespace test {
constexpr float WALK_SPEED = 3.1f;
constexpr float RUN_SPEED = 5.7f;
constexpr float CROUCH_SPEED = 1.2f;
constexpr float JUMP_SPEED = 4.1f;
constexpr float NOCLIP_SPEED = 3.0f;

FpsPlayer::FpsPlayer() {
}

void FpsPlayer::Start() {
}

void FpsPlayer::Update() {
	auto character = GetEntity()->GetComponent<KinematicCharacter>();

	if (!character || !character->IsShapeCreated()) {
		return;
	}

	Vector3f direction;

	if (!Scenes::Get()->GetScene()->IsPaused()) {
		direction.x = Inputs::Get()->GetAxis("strafe")->GetAmount();
		direction.z = Inputs::Get()->GetAxis("forward")->GetAmount();

		if (noclipEnabled) {
			if (Inputs::Get()->GetButton("jump")->IsDown()) {
				direction.y = 1.0f;
			} else if (Inputs::Get()->GetButton("crouch")->IsDown()) {
				direction.y = -1.0f;
			}
		} else {
			if (Inputs::Get()->GetButton("jump")->WasDown() && character->IsOnGround()) {
				character->Jump({0.0f, JUMP_SPEED, 0.0f});
			}
		}

		if (Inputs::Get()->GetButton("noclip")->WasDown()) {
			noclipEnabled = !noclipEnabled;

			if (noclipEnabled) {
				character->SetGravity({});
				character->SetLinearVelocity({});
			} else {
				character->SetGravity(Scenes::Get()->GetScene()->GetSystem<Physics>()->GetGravity());
			}

			Log::Out("Player Noclip: ", std::boolalpha, noclipEnabled, '\n');
		}
	}

	auto cameraRotation = Scenes::Get()->GetScene()->GetCamera()->GetRotation();

	if (auto transform = GetEntity()->GetComponent<Transform>()) {
		transform->SetLocalRotation({0.0f, cameraRotation.y, 0.0f});
	}

	auto walkDirection = direction;
	walkDirection.x = -(direction.z * std::sin(cameraRotation.y) + direction.x * std::cos(cameraRotation.y));
	walkDirection.z = direction.z * std::cos(cameraRotation.y) - direction.x * std::sin(cameraRotation.y);

	//walkDirection = walkDirection.Normalize();
	walkDirection *= Inputs::Get()->GetButton("sprint")->IsDown() ? RUN_SPEED : Inputs::Get()->GetButton("crouch")->IsDown() ? CROUCH_SPEED : WALK_SPEED;
	walkDirection *= noclipEnabled ? NOCLIP_SPEED : 1.0f;
	character->SetWalkDirection(0.02f * walkDirection);
}

const Node &operator>>(const Node &node, FpsPlayer &player) {
	return node;
}

Node &operator<<(Node &node, const FpsPlayer &player) {
	return node;
}
}
