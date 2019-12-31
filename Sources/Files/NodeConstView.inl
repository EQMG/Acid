#pragma once

#include "Node.hpp"

namespace acid {
template<typename T>
T NodeConstView::GetName() const {
	if (!has_value())
		return {};
	return m_value->GetName<T>();
}

template<typename T>
T NodeConstView::Get() const {
	if (!has_value())
		return {};

	return m_value->Get<T>();
}

template<typename T>
T NodeConstView::Get(const T &fallback) const {
	if (!has_value())
		return fallback;

	return m_value->Get<T>(fallback);
}

template<typename T>
bool NodeConstView::Get(T &dest) const {
	if (!has_value())
		return false;

	return m_value->Get<T>(dest);
}

template<typename T, typename K>
bool NodeConstView::Get(T &dest, const K &fallback) const {
	if (!has_value()) {
		dest = fallback;
		return false;
	}

	return m_value->Get<T>(dest, fallback);
}
}
