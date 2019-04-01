#include "AxisCompound.hpp"

namespace acid
{
AxisCompound::AxisCompound(const std::vector<IAxis *> &axes)
{
	for (const auto &axis : axes)
	{
		axis->OnAxis() += [this](float value)
		{
			m_onAxis(GetAmount());
		};

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
