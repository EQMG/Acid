#include "AxisMouse.hpp"

#include "Devices/Mouse.hpp"

namespace acid {
AxisMouse::AxisMouse(uint8_t axis) :
	m_axis(axis) {
	Mouse::Get()->OnPosition().Add([this](Vector2d value) {
		m_onAxis(GetAmount());
	}, this);
}

float AxisMouse::GetAmount() const {
	return m_scale * static_cast<float>(Mouse::Get()->GetPositionDelta()[m_axis]);
}
}
