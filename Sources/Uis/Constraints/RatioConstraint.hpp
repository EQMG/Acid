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
		m_ratio(ratio) {
	}

	int32_t Calculate(const UiConstraints *object, const UiConstraints *parent) const override {
		if constexpr (Type == UiConstraintType::Width) {
			assert(dynamic_cast<RatioConstraint<UiConstraintType::Height> *>(object->GetHeight()) == nullptr &&
				"Ratio constraint can only be applied to one side of a object");
			return object->GetHeight()->Get() * m_ratio;
		} else if constexpr (Type == UiConstraintType::Height) {
			assert(dynamic_cast<RatioConstraint<UiConstraintType::Width> *>(object->GetWidth()) == nullptr &&
				"Ratio constraint can only be applied to one side of a object");
			return object->GetWidth()->Get() * m_ratio;
		}
	}

	float GetRatio() const { return m_ratio; }
	void SetRatio(float ratio) { m_ratio = ratio; }

private:
	float m_ratio;
};
}
