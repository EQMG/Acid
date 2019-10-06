#include "HatJoystick.hpp"

namespace acid {
HatJoystick::HatJoystick(JoystickPort port, JoystickHat hat, const BitMask<JoystickHatValue> &hatFlags) :
	m_port(port),
	m_hat(hat),
	m_hatFlags(hatFlags) {
	Joysticks::Get()->OnHat().Add([this](JoystickPort port, JoystickHat hat, BitMask<JoystickHatValue> value) {
		if (port == m_port && hat == m_hat) {
			m_onAxis(GetAmount());
			auto isDown = IsDown();

			if (!m_lastDown && isDown) {
				m_lastDown = true;
				m_onButton(InputAction::Press, 0);
			} else if (m_lastDown && !isDown) {
				m_lastDown = false;
				m_onButton(InputAction::Release, 0);
			} else if (m_lastDown && isDown) {
				m_onButton(InputAction::Repeat, 0);
			}
		}
	}, static_cast<Button *>(this));
}

float HatJoystick::GetAmount() const {
	auto hat = Joysticks::Get()->GetHat(m_port, m_hat);
	float value = 0.0f;

	if (hat & JoystickHatValue::Up) {
		if (hat & JoystickHatValue::Right)
			value = 0.125f;
		else if (hat & JoystickHatValue::Left)
			value = 0.875f;
		else
			value = 1.0f;
	}
	else if (hat & JoystickHatValue::Down) {
		if (hat & JoystickHatValue::Right)
			value = 0.375f;
		else if (hat & JoystickHatValue::Left)
			value = 0.625f;
		else
			value = 0.5f;
	}
	else if (hat & JoystickHatValue::Right)
		value = 0.25f;
	else if (hat & JoystickHatValue::Left)
		value = 0.75f;
	return m_scale * value;
}

bool HatJoystick::IsDown() const {
	return (Joysticks::Get()->GetHat(m_port, m_hat) & m_hatFlags) ^ m_inverted;
}
}
