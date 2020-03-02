#pragma once

#include "Inputs/Buttons/Button.hpp"
#include "Axis.hpp"

namespace acid {
/**
 * @brief Axis composed of two buttons.
 */
class ACID_EXPORT AxisButton : public Axis::Registrar<AxisButton> {
	inline static const bool Registered = Register("axisButton");
public:
	/**
	 * Creates a new axis button.
	 */
	AxisButton() = default;

	/**
	 * Creates a new axis button.
	 * @param negative When this button is down, the axis is negative.
	 * @param positive When this button is down, the axis is positive.
	 */
	AxisButton(std::unique_ptr<Button> &&negative, std::unique_ptr<Button> &&positive);

	float GetAmount() const override;

	ArgumentDescription GetArgumentDescription() const override;

	const Button *GetNegative() const { return negative.get(); }
	void SetNegative(std::unique_ptr<Button> &&negative);
	const Button *GetPositive() const { return positive.get(); }
	void SetPositive(std::unique_ptr<Button> &&positive);

	friend const Node &operator>>(const Node &node, AxisButton &axisButton);
	friend Node &operator<<(Node &node, const AxisButton &axisButton);

private:
	std::unique_ptr<Button> negative;
	std::unique_ptr<Button> positive;
};
}
