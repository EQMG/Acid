#pragma once

#include "UiAnchor.hpp"
#include "UiConstraints.hpp"

namespace acid {
/**
 * @brief Applied away from a vertex of the parent constraint (default: left top).
 * @tparam Type The constraint enum type.
 */
template<UiConstraintType Type, typename = void>
class PixelConstraint final : public UiConstraint<Type> {
public:
	explicit PixelConstraint(int32_t value = 0) :
		value(value) {
	}
	template<UiConstraintType TType = Type, typename = std::enable_if_t<TType == UiConstraintType::X || TType == UiConstraintType::Y>>
	PixelConstraint(int32_t value, UiAnchor anchor) :
		value(value),
		anchor(anchor) {
	}

	int32_t Calculate(const UiConstraints *object, const UiConstraints *parent) const override {
		if (parent) {
			if constexpr (Type == UiConstraintType::X) {
				return parent->GetX()->Get() + ((parent->GetWidth()->Get() - object->GetWidth()->Get()) * anchor.Get()) + value;
			} else if constexpr (Type == UiConstraintType::Y) {
				return parent->GetY()->Get() + ((parent->GetHeight()->Get() - object->GetHeight()->Get()) * anchor.Get()) + value;
			}
		}
		return value;
	}

	float GetValue() const { return value; }
	void SetValue(float value) { this->value = value; }
	const UiAnchor &GetAnchor() const { return anchor; }
	void SetAnchor(UiAnchor &anchor) { this->anchor = anchor; }

private:
	int32_t value;
	UiAnchor anchor = UiAnchor::Zero;
};
}
