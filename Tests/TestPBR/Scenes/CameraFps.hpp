#pragma once

#include <memory>
#include <Scenes/Camera.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Inputs/ButtonMouse.hpp>

using namespace acid;

namespace test
{
class CameraFps :
	public Camera
{
public:
	CameraFps();

	void Start() override;

	void Update() override;

	void ReflectView(const float &height);

private:
	void CalculateHorizontalAngle();

	void CalculateVerticalAngle();

	void UpdateHorizontalAngle(const float &delta);

	void UpdatePitchAngle(const float &delta);

	void UpdatePosition();

	float m_angleOfElevation;
	float m_angleAroundPlayer;

	Vector3f m_targetPosition;
	Vector3f m_targetRotation;
	float m_targetElevation;
	float m_targetRotationAngle;

	float m_sensitivity;
	MouseButton m_reangleButton;
	AxisJoystick m_joystickVertical;
	AxisJoystick m_joystickHorizontal;

	bool m_paused;
};
}
