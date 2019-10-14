#pragma once

#include "ComponentFilter.hpp"

namespace acid {
template<typename T>
void ComponentFilter::Require() {
	m_required.set(GetComponentTypeId<T>());
	m_excluded.reset(GetComponentTypeId<T>());
}

template<typename T>
void ComponentFilter::Exclude() {
	m_required.reset(GetComponentTypeId<T>());
	m_excluded.set(GetComponentTypeId<T>());
}

template<typename T>
void ComponentFilter::Ignore() {
	m_required.reset(GetComponentTypeId<T>());
	m_excluded.reset(GetComponentTypeId<T>());
}
}
