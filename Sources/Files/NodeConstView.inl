#pragma once

#include "Node.hpp"

namespace acid {
template<typename T>
T NodeConstView::GetName() const {
	if (!has_value())
		return {};
	return value->GetName<T>();
}

template<typename T>
T NodeConstView::Get() const {
	if (!has_value())
		return {};

	return value->Get<T>();
}

template<typename T>
T NodeConstView::Get(const T &fallback) const {
	if (!has_value())
		return fallback;

	return value->Get<T>(fallback);
}

template<typename T>
bool NodeConstView::Get(T &dest) const {
	if (!has_value())
		return false;

	return value->Get<T>(dest);
}

template<typename T, typename K>
bool NodeConstView::Get(T &dest, const K &fallback) const {
	if (!has_value()) {
		dest = fallback;
		return false;
	}

	return value->Get<T>(dest, fallback);
}
}
