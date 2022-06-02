#include "FpsCamera.hpp"

#include <Inputs/Inputs.hpp>
#include <Scenes/Scenes.hpp>
#include <Maths/Maths.hpp>
#include <Graphics/Graphics.hpp>
#include "FpsPlayer.hpp"

namespace test {
constexpr static Vector3f ViewOffset(0.0f, 1.8f, 0.0f);

FpsCamera::FpsCamera() {
	nearPlane = 0.1f;
	farPlane = 4098.0f;
	fieldOfView = Maths::Radians(70.0f);
}

void FpsCamera::Start() {
}

void FpsCamera::Update() {
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	if (auto scenePlayer = Scenes::Get()->GetScene()->GetComponent<FpsPlayer>()) {
		if (auto transformPlayer = scenePlayer->GetEntity()->GetComponent<Transform>()) {
			velocity = (transformPlayer->GetPosition() - position) / delta;
			position = transformPlayer->GetPosition() + ViewOffset;
		}
	}

	if (!Scenes::Get()->GetScene()->IsPaused()) {
		auto rotationDelta = Windows::Get()->GetWindow(0)->IsCursorHidden() * Vector2f(Inputs::Get()->GetAxis("mouseX")->GetAmount(),
			Inputs::Get()->GetAxis("mouseY")->GetAmount());

		rotation.y += rotationDelta.x;
		rotation.x += rotationDelta.y;
		rotation.x = std::clamp(rotation.x, Maths::Radians(90.0f), Maths::Radians(270.0f));
	}

	viewMatrix = Matrix4::ViewMatrix(position, rotation);
	projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView(), Windows::Get()->GetWindow(0)->GetAspectRatio(), GetNearPlane(), GetFarPlane());

	viewFrustum.Update(viewMatrix, projectionMatrix);
	viewRay.Update(position, {0.5f, 0.5f}, viewMatrix, projectionMatrix);

	//auto raytest = Scenes::Get()->GetScene()->GetSystem<Physics>()->Raytest(viewRay.GetOrigin(), viewRay.GetPointOnRay(20.0f));
	//Log::Out((raytest.HasHit() ? raytest.GetParent()->GetName() : ""), ": ", raytest.GetPointWorld().Distance(viewRay.GetOrigin()), '\n');
}
}
