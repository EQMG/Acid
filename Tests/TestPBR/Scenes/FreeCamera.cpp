#include "FreeCamera.hpp"

#include <Inputs/Inputs.hpp>
#include <Maths/Maths.hpp>
#include <Scenes/Scenes.hpp>

namespace test {
constexpr float WALK_SPEED = 3.0f;
constexpr float RUN_SPEED = 7.0f;
constexpr Vector3f DAMP(20.0f, 20.0f, 20.0f);

FreeCamera::FreeCamera() {
	nearPlane = 0.1f;
	farPlane = 4098.0f;
	fieldOfView = Maths::Radians(70.0f);
}

void FreeCamera::Start() {
}

void FreeCamera::Update() {
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	if (!Scenes::Get()->GetScene()->IsPaused()) {
		Vector3f positionDelta;

		if (!Scenes::Get()->GetScene()->IsPaused()) {
			positionDelta.x = Inputs::Get()->GetAxis("strafe")->GetAmount();
			positionDelta.y = Inputs::Get()->GetAxis("vertical")->GetAmount();
			positionDelta.z = Inputs::Get()->GetAxis("forward")->GetAmount();
		}

		positionDelta *= Inputs::Get()->GetButton("sprint")->IsDown() ? -RUN_SPEED : -WALK_SPEED;
		velocity = velocity.SmoothDamp(positionDelta, delta * DAMP);

		auto rotationDelta = Windows::Get()->GetWindow(0)->IsCursorHidden() * Vector2f(Inputs::Get()->GetAxis("mouseX")->GetAmount(),
			Inputs::Get()->GetAxis("mouseY")->GetAmount());

		rotation.y += rotationDelta.x;
		rotation.x += rotationDelta.y;
		rotation.x = std::clamp(rotation.x, Maths::Radians(90.0f), Maths::Radians(270.0f));

		position.x += -(velocity.z * std::sin(rotation.y) + velocity.x * std::cos(rotation.y)) * delta;
		position.y += velocity.y * delta;
		position.z += (velocity.z * std::cos(rotation.y) - velocity.x * std::sin(rotation.y)) * delta;
	}

	viewMatrix = Matrix4::ViewMatrix(position, rotation);
	projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView(), Windows::Get()->GetWindow(0)->GetAspectRatio(), GetNearPlane(), GetFarPlane());

	viewFrustum.Update(viewMatrix, projectionMatrix);
	viewRay.Update(position, {0.5f, 0.5f}, viewMatrix, projectionMatrix);
}
}
