#pragma once

#include "IAxis.hpp"
#include "IButton.hpp"

namespace acid
{
/// <summary>
/// Axis composed of two buttons.
/// </summary>
class ACID_EXPORT AxisButton :
	public IAxis
{
public:
	/// <summary>
	/// A template used to create a axis button of a single type.
	/// </summary>
	/// <param name="args"> The arguments to pass to T. </param>
	/// <param name="T"> The type of buttons to create. </param>
	/// <param name="Args"> The values passed to each button. </param>
	template<class T, typename... Args>
	static AxisButton *Create(Args &&... args)
	{
		std::vector<IButton *> buttons;

		for (const auto &x : {args...})
		{
			buttons.emplace_back(new T(x));
		}

		return new AxisButton(buttons.size() > 0 ? buttons[0] : nullptr,
			buttons.size() > 1 ? buttons[1] : nullptr);
	}

	/// <summary>
	/// Creates a new axis button.
	/// </summary>
	/// <param name="negative"> When this button is down, the axis is negative. </param>
	/// <param name="positive"> When this button is down, the axis is positive. </param>
	AxisButton(IButton *negative, IButton *positive);

	float GetAmount() const override;

private:
	std::unique_ptr<IButton> m_negative;
	std::unique_ptr<IButton> m_positive;
};
}
