#include "ComponentFilter.hpp"

namespace acid {
ComponentFilter::ComponentFilter() :
	m_filter(std::make_unique<FilterNone>()) {
}

bool ComponentFilter::Check(const Mask &mask) const {
	return m_filter->Compute(mask);
}

void ComponentFilter::Set(const Filter &filter) {
	m_filter = filter.Clone();
}

std::unique_ptr<Filter> FilterNone::Clone() const {
	return std::make_unique<FilterNone>();
}

bool FilterNone::Compute(const ComponentFilter::Mask &mask) const {
	return true;
}

LogicalAnd::LogicalAnd(const Filter &left, const Filter &right):
	m_left(left.Clone()),
	m_right(right.Clone()) {
}

std::unique_ptr<Filter> LogicalAnd::Clone() const {
	return std::make_unique<LogicalAnd>(*m_left, *m_right);
}

bool LogicalAnd::Compute(const ComponentFilter::Mask &mask) const {
	return m_left->Compute(mask) && m_right->Compute(mask);
}

LogicalOr::LogicalOr(const Filter &left, const Filter &right):
	m_left(left.Clone()),
	m_right(right.Clone()) {
}

std::unique_ptr<Filter> LogicalOr::Clone() const {
	return std::make_unique<LogicalOr>(*m_left, *m_right);
}

bool LogicalOr::Compute(const ComponentFilter::Mask &mask) const {
	return m_left->Compute(mask) || m_right->Compute(mask);
}

LogicalNot::LogicalNot(const Filter &right):
	m_right(right.Clone()) {
}

bool LogicalNot::Compute(const ComponentFilter::Mask &mask) const {
	return !m_right->Compute(mask);
}

std::unique_ptr<Filter> LogicalNot::Clone() const {
	return std::make_unique<LogicalNot>(*m_right);
}
}
