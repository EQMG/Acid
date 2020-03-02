#pragma once

#include <cassert>

#include "UiConstraints.hpp"

namespace acid {
/**
 * @brief Sets the width or height of a rectangle to a ratio of the opposite sides value.
 * @tparam Type The constraint enum type.
 */
template<UiConstraintType Type,
	typename = std::enable_if_t<Type == UiConstraintType::Width || Type == UiConstraintType::Height>>
class RatioConstraint final : public UiConstraint<Type> {
public:
	explicit RatioConstraint(float ratio = 1.0f) :
		ratio(ratio) {
	}

	int32_t Calculate(const UiConstraints *object, const UiConstraints *parent) const override {
		if constexpr (Type == UiConstraintType::Width) {
			assert(dynamic_cast<RatioConstraint<UiConstraintType::Height> *>(object->GetHeight()) == nullptr &&
				"Ratio constraint can only be applied to one side of a object");
			return object->GetHeight()->Get() * ratio;
		} else if constexpr (Type == UiConstraintType::Height) {
			assert(dynamic_cast<RatioConstraint<UiConstraintType::Width> *>(object->GetWidth()) == nullptr &&
				"Ratio constraint can only be applied to one side of a object");
			return object->GetWidth()->Get() * ratio;
		}
	}

	float GetRatio() const { return ratio; }
	void SetRatio(float ratio) { this->ratio = ratio; }

private:
	float ratio;
};
}
