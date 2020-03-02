#include "CameraFree.hpp"

#include <Devices/Mouse.hpp>
#include <Inputs/Input.hpp>
#include <Maths/Maths.hpp>
#include <Scenes/Scenes.hpp>

namespace test {
constexpr float WALK_SPEED = 3.0f;
constexpr float RUN_SPEED = 7.0f;
constexpr Vector3f DAMP(20.0f, 20.0f, 20.0f);

CameraFree::CameraFree() {
	nearPlane = 0.1f;
	farPlane = 4098.0f;
	fieldOfView = Maths::Radians(70.0f);
}

void CameraFree::Start() {
}

void CameraFree::Update() {
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	if (!Scenes::Get()->IsPaused()) {
		Vector3f positionDelta;

		if (!Scenes::Get()->IsPaused()) {
			positionDelta.x = Input::Get()->GetAxis("strafe")->GetAmount();
			positionDelta.y = Input::Get()->GetAxis("vertical")->GetAmount();
			positionDelta.z = Input::Get()->GetAxis("forward")->GetAmount();
		}

		positionDelta *= Input::Get()->GetButton("sprint")->IsDown() ? -RUN_SPEED : -WALK_SPEED;
		velocity = velocity.SmoothDamp(positionDelta, delta * DAMP);

		auto rotationDelta = Mouse::Get()->IsCursorHidden() * Vector2f(Input::Get()->GetAxis("mouseX")->GetAmount(),
			Input::Get()->GetAxis("mouseY")->GetAmount());

		rotation.y += rotationDelta.x;
		rotation.x += rotationDelta.y;
		rotation.x = std::clamp(rotation.x, Maths::Radians(90.0f), Maths::Radians(270.0f));

		position.x += -(velocity.z * std::sin(rotation.y) + velocity.x * std::cos(rotation.y)) * delta;
		position.y += velocity.y * delta;
		position.z += (velocity.z * std::cos(rotation.y) - velocity.x * std::sin(rotation.y)) * delta;
	}

	viewMatrix = Matrix4::ViewMatrix(position, rotation);
	projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView(), Window::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

	viewFrustum.Update(viewMatrix, projectionMatrix);
	viewRay.Update(position, {0.5f, 0.5f}, viewMatrix, projectionMatrix);
}
}
