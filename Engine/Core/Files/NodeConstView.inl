#pragma once

#include "Node.hpp"

namespace acid {
template<typename T>
T NodeConstView::Get() const {
	if (!has_value())
		return {};

	return value->Get<T>();
}

template<typename T>
T NodeConstView::GetWithFallback(const T &fallback) const {
	if (!has_value())
		return fallback;

	return value->GetWithFallback<T>(fallback);
}

template<typename T>
bool NodeConstView::Get(T &dest) const {
	if (!has_value())
		return false;

	return value->Get<T>(dest);
}

template<typename T, typename K>
bool NodeConstView::GetWithFallback(T &dest, const K &fallback) const {
	if (!has_value()) {
		dest = fallback;
		return false;
	}

	return value->GetWithFallback<T>(dest, fallback);
}

template<typename T>
bool NodeConstView::Get(T &&dest) const {
	if (!has_value())
		return false;

	return value->Get<std::remove_reference_t<T>>(std::move(dest));
}

template<typename T, typename K>
bool NodeConstView::GetWithFallback(T &&dest, const K &fallback) const {
	if (!has_value()) {
		dest = fallback;
		return false;
	}

	return value->GetWithFallback<std::remove_reference_t<T>>(std::move(dest), fallback);
}
}
