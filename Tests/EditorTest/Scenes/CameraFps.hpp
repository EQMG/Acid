#pragma once

#include <Scenes/Camera.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Inputs/ButtonMouse.hpp>

using namespace acid;

namespace test {
class CameraFps : public Camera {
public:
	CameraFps();

	void Start() override;
	void Update() override;

private:
	float m_sensitivity;
	AxisJoystick m_joystickVertical;
	AxisJoystick m_joystickHorizontal;
};
}
