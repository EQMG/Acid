#pragma once

#include "Inputs/InputAxis.hpp"
#include "Inputs/InputButton.hpp"

namespace acid {
/**
 * @brief InputButton composed of a axis.
 */
class ACID_EXPORT AxisInputButton : public InputButton::Registrar<AxisInputButton> {
	inline static const bool Registered = Register("axis");
public:
	/**
	 * Creates a new button axis.
	 * @param axis The axis to sample.
	 * @param min Lower axis value bound.
	 * @param max Upper axis value bound.
	 */
	explicit AxisInputButton(std::unique_ptr<InputAxis> &&axis = nullptr, float min = 0.1f, float max = 1.0f);

	bool IsDown() const override;

	InputAxis::ArgumentDescription GetArgumentDescription() const override;

	const InputAxis *GetAxis() const { return axis.get(); }
	void SetAxis(std::unique_ptr<InputAxis> &&axis);

	float GetMin() const { return min; }
	void SetMin(float min) { this->min = min; }
	float GetMax() const { return max; }
	void SetMax(float max) { this->max = max; }

	friend const Node &operator>>(const Node &node, AxisInputButton &inputButton);
	friend Node &operator<<(Node &node, const AxisInputButton &inputButton);

private:
	std::unique_ptr<InputAxis> axis;
	float min, max;
};
}
