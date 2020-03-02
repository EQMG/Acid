#pragma once

#include "Axis.hpp"

namespace acid {
/**
 * @brief Axis input from a mouse.
 */
class ACID_EXPORT AxisMouse : public Axis::Registrar<AxisMouse> {
	inline static const bool Registered = Register("axisMouse");
public:
	/**
	 * Creates a new axis mouse.
	 * @param axis The axis on the mouse delta is being checked.
	 */
	explicit AxisMouse(uint8_t axis = 0);

	float GetAmount() const override;

	ArgumentDescription GetArgumentDescription() const override;

	uint8_t GetAxis() const { return axis; }
	void SetAxis(uint8_t axis) { this->axis = axis; }

	friend const Node &operator>>(const Node &node, AxisMouse &axisMouse);
	friend Node &operator<<(Node &node, const AxisMouse &axisMouse);

private:
	uint8_t axis;
};
}
