#pragma once

#include "Inputs/InputButton.hpp"
#include "Inputs/InputAxis.hpp"

namespace acid {
/**
 * @brief InputAxis composed of two buttons.
 */
class ACID_EXPORT ButtonInputAxis : public InputAxis::Registrar<ButtonInputAxis> {
	static const bool Registered;
public:
	/**
	 * Creates a new axis button.
	 */
	ButtonInputAxis() = default;

	/**
	 * Creates a new axis button.
	 * @param negative When this button is down, the axis is negative.
	 * @param positive When this button is down, the axis is positive.
	 */
	ButtonInputAxis(std::unique_ptr<InputButton> &&negative, std::unique_ptr<InputButton> &&positive);

	float GetAmount() const override;

	ArgumentDescription GetArgumentDescription() const override;

	const InputButton *GetNegative() const { return negative.get(); }
	void SetNegative(std::unique_ptr<InputButton> &&negative);
	const InputButton *GetPositive() const { return positive.get(); }
	void SetPositive(std::unique_ptr<InputButton> &&positive);

	friend const Node &operator>>(const Node &node, ButtonInputAxis &inputAxis);
	friend Node &operator<<(Node &node, const ButtonInputAxis &inputAxis);

private:
	std::unique_ptr<InputButton> negative;
	std::unique_ptr<InputButton> positive;
};
}
