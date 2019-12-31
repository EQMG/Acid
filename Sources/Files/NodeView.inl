#pragma once

#include "Node.hpp"

namespace acid {
template<typename T>
void NodeView::SetName(const T &value) {
	return get()->SetName<T>(value);
}

template<typename T>
void NodeView::Set(const T &value) {
	get()->Set<T>(value);
}

template<typename T>
Node &NodeView::operator=(const T &rhs) {
	return *get() = rhs;
}
}
