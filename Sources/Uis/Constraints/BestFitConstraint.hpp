#pragma once

#include "UiConstraints.hpp"

namespace acid {
/**
 * @brief Sets the width or height of a rectangle to best fit inside of the parents constraints.
 * @tparam Type The constraint enum type.
 */
template<UiConstraintType Type,
	typename = std::enable_if_t<Type == UiConstraintType::Width || Type == UiConstraintType::Height>>
class BestFitConstraint final : public UiConstraint<Type> {
public:
	explicit BestFitConstraint(float ratio = 1.0f) :
		ratio(ratio) {
	}

	int32_t Calculate(const UiConstraints *object, const UiConstraints *parent) const override {
		if constexpr (Type == UiConstraintType::Width) {
			return parent->GetWidth()->Get() * ratio;
		} else if constexpr (Type == UiConstraintType::Height) {
			return parent->GetHeight()->Get() / ratio;
		}
	}

	float GetRatio() const { return ratio; }
	void SetRatio(float ratio) { this->ratio = ratio; }
	
private:
	float ratio;
};
}
