#pragma once

#include "Inputs/Buttons/IButton.hpp"
#include "IAxis.hpp"

namespace Flounder
{
	/// <summary>
	/// Axis composed of two buttons.
	/// </summary>
	class F_EXPORT AxisButton :
		public IAxis
	{
	private:
		IButton *m_negative;
		IButton *m_positive;
	public:
		/// <summary>
		/// Creates a new axis button.
		/// </summary>
		/// <param name="negative"> When this button is down, the axis is negative. </param>
		/// <param name="positive"> When this button is down, the axis is positive. </param>
		AxisButton(IButton *negative, IButton *positive);

		/// <summary>
		/// Deconstructor for the axis joystick.
		/// </summary>
		~AxisButton();

		float GetAmount() const override;
	};
}
