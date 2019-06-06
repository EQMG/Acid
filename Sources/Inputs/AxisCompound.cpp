#include "AxisCompound.hpp"

namespace acid
{
/*AxisCompound::AxisCompound(std::vector<std::unique_ptr<Axis>> &&axes) :
	m_axes{std::move(axes)}
{
	ConnectAxes();
}*/

float AxisCompound::GetAmount() const
{
	auto result = 0.0f;

	for (const auto &axis : m_axes)
	{
		result += axis->GetAmount();
	}

	return std::clamp(result, -1.0f, 1.0f);
}

void AxisCompound::ConnectAxes()
{
	for (auto &axis : m_axes)
	{
		axis->OnAxis().Add([this](float value)
		{
			m_onAxis(GetAmount());
		}, this);
	}
}
}
