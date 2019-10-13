#pragma once

#include "Inputs/Axes/Axis.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Button composed of a axis.
 */
class ACID_EXPORT ButtonAxis : public Button::Registrar<ButtonAxis> {
public:
	/**
	 * Creates a new button axis.
	 */
	ButtonAxis() = default;

	/**
	 * Creates a new button axis.
	 * @param axis The axis to sample.
	 * @param min Lower axis value bound.
	 * @param max Upper axis value bound.
	 */
	ButtonAxis(std::unique_ptr<Axis> &&axis, float min = 0.1f, float max = 1.0f);

	bool IsDown() const override;

	Axis::ArgumentDescription GetArgumentDescription() const override;

	const Axis *GetAxis() const { return m_axis.get(); }

	float GetMin() const { return m_min; }
	void SetMin(float min) { m_min = min; }
	float GetMax() const { return m_max; }
	void SetMax(float max) { m_max = max; }

	friend const Node &operator>>(const Node &node, ButtonAxis &buttonAxis);
	friend Node &operator<<(Node &node, const ButtonAxis &buttonAxis);

private:
	static bool registered;

	std::unique_ptr<Axis> m_axis;
	float m_min, m_max;
};
}
