#pragma once

#include "Axis.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Axis composed of two buttons.
 */
class ACID_EXPORT AxisButton : public Axis {
public:
	/**
	 * Creates a new axis button.
	 * @param negative When this button is down, the axis is negative.
	 * @param positive When this button is down, the axis is positive.
	 */
	AxisButton(std::unique_ptr<Button> &&negative, std::unique_ptr<Button> &&positive);

	float GetAmount() const override;

private:
	std::unique_ptr<Button> m_negative;
	std::unique_ptr<Button> m_positive;
};
}
