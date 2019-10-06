#pragma once

#include "Axis.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Button composed of a axis.
 */
class ACID_EXPORT ButtonAxis : public Button {
public:
	/**
	 * Creates a new button axis.
	 * @param axis The axis to sample.
	 * @param min Lower axis value bound.
	 * @param max Upper axis value bound.
	 */
	ButtonAxis(std::unique_ptr<Axis> &&axis, float min = 0.1f, float max = 1.0f);

	bool IsDown() const override;

private:
	std::unique_ptr<Axis> m_axis;
	float m_min, m_max;
};
}
