#include "AxisCompound.hpp"

#include "Maths/Maths.hpp"

namespace acid
{
	AxisCompound::AxisCompound(const std::vector<std::shared_ptr<IAxis>> &axes) :
		m_axes(axes)
	{
	}

	AxisCompound::~AxisCompound()
	{
	}

	float AxisCompound::GetAmount() const
	{
		float result = 0.0f;

		for (auto &axis : m_axes)
		{
			result += axis->GetAmount();
		}

		return std::clamp(result, -1.0f, 1.0f);
	}
}
