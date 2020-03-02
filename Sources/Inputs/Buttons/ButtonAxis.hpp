#pragma once

#include "Inputs/Axes/Axis.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Button composed of a axis.
 */
class ACID_EXPORT ButtonAxis : public Button::Registrar<ButtonAxis> {
	inline static const bool Registered = Register("buttonAxis");
public:
	/**
	 * Creates a new button axis.
	 * @param axis The axis to sample.
	 * @param min Lower axis value bound.
	 * @param max Upper axis value bound.
	 */
	explicit ButtonAxis(std::unique_ptr<Axis> &&axis = nullptr, float min = 0.1f, float max = 1.0f);

	bool IsDown() const override;

	Axis::ArgumentDescription GetArgumentDescription() const override;

	const Axis *GetAxis() const { return axis.get(); }
	void SetAxis(std::unique_ptr<Axis> &&axis);

	float GetMin() const { return min; }
	void SetMin(float min) { this->min = min; }
	float GetMax() const { return max; }
	void SetMax(float max) { this->max = max; }

	friend const Node &operator>>(const Node &node, ButtonAxis &buttonAxis);
	friend Node &operator<<(Node &node, const ButtonAxis &buttonAxis);

private:
	std::unique_ptr<Axis> axis;
	float min, max;
};
}
