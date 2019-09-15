#pragma once

#include <Scenes/Camera.hpp>
#include <Inputs/AxisJoystick.hpp>
#include <Inputs/ButtonMouse.hpp>
#include <Inputs/AxisCompound.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Inputs/AxisButton.hpp>

using namespace acid;

namespace test {
class CameraFree :
	public Camera {
public:
	CameraFree();

	void Start() override;

	void Update() override;

private:
	float m_sensitivity;
	AxisJoystick m_joystickVertical;
	AxisJoystick m_joystickHorizontal;

	AxisCompound m_inputForward;
	AxisCompound m_inputStrafe;
	AxisButton m_inputVertical;
	ButtonCompound m_inputSprint;
};
}
