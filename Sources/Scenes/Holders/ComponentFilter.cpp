#include "ComponentFilter.hpp"

namespace acid {
bool ComponentFilter::Check(const Mask &mask) const {
	const auto excludeMask = m_excluded & mask;

	// Checks if there is an excluded component.
	if (excludeMask.any()) {
		return false;
	}

	for (std::size_t i = 0; i < m_required.size(); ++i) {
		if (m_required[i] && !mask[i]) {
			// A required component is missing.
			return false;
		}
	}

	return true;
}

void ComponentFilter::ExcludeNotRequired() noexcept {
	m_excluded = ~m_required;
}

void ComponentFilter::ExcludeAll() noexcept {
	m_required.reset();
	m_excluded.set();
}
}
