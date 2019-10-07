#include "CameraFree.hpp"

#include <Devices/Mouse.hpp>
#include <Inputs/Input.hpp>
#include <Maths/Maths.hpp>
#include <Scenes/Scenes.hpp>

namespace test {
const float WALK_SPEED = 3.0f;
const float RUN_SPEED = 7.0f;
const Vector3f DAMP(20.0f, 20.0f, 20.0f);

CameraFree::CameraFree() {
	m_nearPlane = 0.1f;
	m_farPlane = 4098.0f;
	m_fieldOfView = Maths::Radians(70.0f);
}

void CameraFree::Start() {
}

void CameraFree::Update() {
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	if (!Scenes::Get()->IsPaused()) {
		Vector3f positionDelta;

		if (!Scenes::Get()->IsPaused()) {
			positionDelta.m_x = Input::Get()->GetAxis("strafe")->GetAmount();
			positionDelta.m_y = Input::Get()->GetAxis("vertical")->GetAmount();
			positionDelta.m_z = Input::Get()->GetAxis("forward")->GetAmount();
		}

		positionDelta *= Input::Get()->GetButton("sprint")->IsDown() ? -RUN_SPEED : -WALK_SPEED;
		m_velocity = m_velocity.SmoothDamp(positionDelta, delta * DAMP);

		auto rotationDelta = Mouse::Get()->IsCursorHidden() * Vector2f(Input::Get()->GetAxis("mouseX")->GetAmount(),
			Input::Get()->GetAxis("mouseY")->GetAmount());

		m_rotation.m_y += rotationDelta.m_x;
		m_rotation.m_x += rotationDelta.m_y;
		m_rotation.m_x = std::clamp(m_rotation.m_x, Maths::Radians(90.0f), Maths::Radians(270.0f));

		m_position.m_x += -(m_velocity.m_z * std::sin(m_rotation.m_y) + m_velocity.m_x * std::cos(m_rotation.m_y)) * delta;
		m_position.m_y += m_velocity.m_y * delta;
		m_position.m_z += (m_velocity.m_z * std::cos(m_rotation.m_y) - m_velocity.m_x * std::sin(m_rotation.m_y)) * delta;
	}

	m_viewMatrix = Matrix4::ViewMatrix(m_position, m_rotation);
	m_projectionMatrix = Matrix4::PerspectiveMatrix(GetFieldOfView(), Window::Get()->GetAspectRatio(), GetNearPlane(), GetFarPlane());

	m_viewFrustum.Update(m_viewMatrix, m_projectionMatrix);
	m_viewRay.Update(m_position, {0.5f, 0.5f}, m_viewMatrix, m_projectionMatrix);
}
}
