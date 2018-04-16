#include "AxisCompound.hpp"

#include "Maths/Maths.hpp"

namespace Flounder
{
	AxisCompound::AxisCompound(const std::vector<IAxis *> &axes) :
		IAxis(),
		m_axes(std::vector<IAxis *>(axes))
	{
	}

	AxisCompound::~AxisCompound()
	{
	}

	float AxisCompound::GetAmount() const
	{
		float result = 0.0f;

		for (auto axis : m_axes)
		{
			result += axis->GetAmount();
		}

		return Maths::Clamp(result, -1.0f, 1.0f);
	}
}
