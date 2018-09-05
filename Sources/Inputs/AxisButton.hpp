#pragma once

#include <memory>
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
	private:
		std::shared_ptr<IButton> m_negative;
		std::shared_ptr<IButton> m_positive;
	public:
		/// <summary>
		/// Creates a new axis button.
		/// </summary>
		/// <param name="negative"> When this button is down, the axis is negative. </param>
		/// <param name="positive"> When this button is down, the axis is positive. </param>
		AxisButton(const std::shared_ptr<IButton> &negative, const std::shared_ptr<IButton> &positive);

		~AxisButton();

		float GetAmount() const override;
	};
}
