#pragma once

#include <algorithm>

#include "UiAnchor.hpp"
#include "UiConstraints.hpp"

namespace acid {
/**
 * @brief Position relative to a vertex of the parent (default: left top), or size as percentage of the parent.
 * @tparam Type The constraint enum type.
 */
template<UiConstraintType Type, typename = void>
class RelativeConstraint final : public UiConstraint<Type> {
public:
	explicit RelativeConstraint(float value = 0.0f) :
		m_value(value) {
	}
	template<UiConstraintType TType = Type, typename = std::enable_if_t<TType == UiConstraintType::X || TType == UiConstraintType::Y>>
	RelativeConstraint(float value, UiAnchor anchor) :
		m_value(value),
		m_anchor(anchor) {
	}

	int32_t Calculate(const UiConstraints *object, const UiConstraints *parent) const override {
		if constexpr (Type == UiConstraintType::X) {
			return parent->GetX()->Get() + ((parent->GetWidth()->Get() - object->GetWidth()->Get()) * m_anchor.Get()) + (parent->GetWidth()->Get() * m_value);
		} else if constexpr (Type == UiConstraintType::Y) {
			return parent->GetY()->Get() + ((parent->GetHeight()->Get() - object->GetHeight()->Get()) * m_anchor.Get()) + (parent->GetHeight()->Get() * m_value);
		} else if constexpr (Type == UiConstraintType::Width) {
			return parent->GetWidth()->Get() * m_value;
		} else if constexpr (Type == UiConstraintType::Height) {
			return parent->GetHeight()->Get() * m_value;
		}
	}

	float m_value;
	UiAnchor m_anchor = UiAnchor::Zero;
};
}
