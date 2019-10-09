#include "ButtonAxis.hpp"

namespace acid {
ButtonAxis::ButtonAxis(std::unique_ptr<Axis> &&axis, float min, float max) :
	m_axis(std::move(axis)),
	m_min(min),
	m_max(max) {
	/*m_axis->OnAxis().Add([this](float value) {
		m_onButton(IsDown());
	}, this);*/
}

bool ButtonAxis::IsDown() const {
	float amount = m_axis->GetAmount();
	return m_min < amount && amount < m_max;
}
}
