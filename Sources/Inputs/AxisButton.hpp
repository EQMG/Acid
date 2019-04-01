#pragma once

#include "IAxis.hpp"
#include "IButton.hpp"

namespace acid
{
/**
 * @brief Axis composed of two buttons.
 */
class ACID_EXPORT AxisButton :
	public IAxis
{
public:
	/**
	 * A template used to create a axis button of a single type.
	 * @tparam T The type of button axis to create.
	 * @tparam Args The values passed to each button axis.
	 * @param args The arguments to pass to T
	 * @return The created button axis.
	 */
	template<class T, typename... Args>
	static AxisButton *Create(Args &&... args)
	{
		std::vector<IButton *> buttons;

		for (const auto &x : { args... })
		{
			buttons.emplace_back(new T(x));
		}

		return new AxisButton(buttons.empty() ? nullptr : buttons[0], buttons.size() > 1 ? buttons[1] : nullptr);
	}

	/**
	 * Creates a new axis button.
	 * @param negative When this button is down, the axis is negative.
	 * @param positive When this button is down, the axis is positive.
	 */
	AxisButton(IButton *negative, IButton *positive);

	float GetAmount() const override;

private:
	std::unique_ptr<IButton> m_negative;
	std::unique_ptr<IButton> m_positive;
};
}
