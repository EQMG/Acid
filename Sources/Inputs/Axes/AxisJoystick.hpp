#pragma once

#include "Axis.hpp"
#include "Devices/Joysticks.hpp"

namespace acid {
/**
 * @brief Axis input from a joystick input device.
 */
class ACID_EXPORT AxisJoystick : public Axis::Registrar<AxisJoystick> {
	inline static const bool Registered = Register("axisJoystick");
public:
	/**
	 * Creates a new axis joystick.
	 * @param port The joystick port.
	 * @param axis The axis on the joystick being checked.
	 */
	explicit AxisJoystick(JoystickPort port = 0, JoystickAxis axis = 0);

	float GetAmount() const override;

	ArgumentDescription GetArgumentDescription() const override;

	bool IsConnected() const;

	JoystickPort GetPort() const { return port; }
	void SetPort(JoystickPort port) { this->port = port; }

	JoystickAxis GetAxis() const { return axis; }
	void SetAxis(JoystickAxis axis) { this->axis = axis; }

	friend const Node &operator>>(const Node &node, AxisJoystick &axisJoystick);
	friend Node &operator<<(Node &node, const AxisJoystick &axisJoystick);

private:
	JoystickPort port;
	JoystickAxis axis;
};
}
