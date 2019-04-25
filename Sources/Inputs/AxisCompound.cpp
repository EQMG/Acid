#include "AxisCompound.hpp"

namespace acid
{
AxisCompound::AxisCompound(const std::vector<Axis *> &axes)
{
	for (const auto &axis : axes)
	{
		axis->OnAxis().Add([this](float value)
		{
			m_onAxis(GetAmount());
		}, std::ref(*this));

		m_axes.emplace_back(axis);
	}
}

float AxisCompound::GetAmount() const
{
	auto result = 0.0f;

	for (const auto &axis : m_axes)
	{
		result += axis->GetAmount();
	}

	return std::clamp(result, -1.0f, 1.0f);
}
}
