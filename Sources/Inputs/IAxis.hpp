#pragma once

#include "Helpers/Delegate.hpp"
#include "StdAfx.hpp"

namespace acid
{
/**
 * Interface for an axis based input device.
 */
class ACID_EXPORT IAxis
{
public:
	virtual ~IAxis() = default;

	/**
	 * Gets the current value along the axis. -1 is smallest input, 1 is largest input.
	 * @return The current value of the axis in the range (-1, 1).
	 */
	virtual float GetAmount() const = 0;

	/**
	 * Called when the axis changes value.
	 * @return The delegate.
	 */
	Delegate<void(float)> &OnAxis() { return m_onAxis; }

protected:
	Delegate<void(float)> m_onAxis;
};
}
