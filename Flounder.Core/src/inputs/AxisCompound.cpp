#include "axiscompound.hpp"

namespace Flounder
{
	AxisCompound::AxisCompound(const int n_args, ...) :
		IAxis(),
		m_count(n_args),
		m_axes(new IAxis*[n_args])
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_axes[i] = va_arg(ap, IAxis*);
		}

		va_end(ap);
	}

	AxisCompound::~AxisCompound()
	{
		delete m_axes;
	}

	float AxisCompound::GetAmount() const
	{
		float result = 0.0f;

		for (int i = 0; i < m_count; i++)
		{
			result += m_axes[i]->GetAmount();
		}

		return Maths::Clamp(result, -1.0f, 1.0f);
	}
}
