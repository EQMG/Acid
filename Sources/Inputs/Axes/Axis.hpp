#pragma once

#include "Helpers/Delegate.hpp"
#include "Helpers/Factory.hpp"

namespace acid {
/**
 * @brief Interface for an axis based input device.
 */
class ACID_EXPORT Axis : public Factory<Axis>, public virtual Observer {
public:
	virtual ~Axis() = default;

	/**
	 * Gets the current value along the axis. -1 is smallest input, 1 is largest input.
	 * @return The current value of the axis in the range (-1, 1).
	 */
	virtual float GetAmount() const { return 0.0f; }

	/**
	 * Called when the axis changes value.
	 * @return The delegate.
	 */
	Delegate<void(float)> &OnAxis() { return m_onAxis; }

	float GetScale() const { return m_scale; }
	void SetScale(float scale) { m_scale = scale; }

protected:
	Delegate<void(float)> m_onAxis;
	float m_scale = 1.0f;
};
}
