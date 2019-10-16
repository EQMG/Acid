#pragma once

#include "ComponentFilter.hpp"

namespace acid {
template<typename T>
std::unique_ptr<Filter> FilterMatch<T>::Clone() const {
	return std::make_unique<FilterMatch<T>>();
}

template<typename T>
bool FilterMatch<T>::Compute(const ComponentFilter::Mask &mask) const {
	return mask[GetComponentTypeId<T>()];
}

inline LogicalAnd operator&&(const Filter &lhs, const Filter &rhs) {
	return LogicalAnd(lhs, rhs);
}

inline LogicalOr operator||(const Filter &lhs, const Filter &rhs) {
	return LogicalOr(lhs, rhs);
}

inline LogicalNot operator!(const Filter &rhs) {
	return LogicalNot(rhs);
}
}
