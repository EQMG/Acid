#include "axiscompound.hpp"

namespace Flounder
{
	axiscompound::axiscompound(const int n_args, ...) :
		iaxis(),
		m_count(n_args),
		m_axes(new iaxis*[n_args])
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_axes[i] = va_arg(ap, iaxis*);
		}

		va_end(ap);
	}

	axiscompound::~axiscompound()
	{
		delete m_axes;
	}

	float axiscompound::getAmount() const
	{
		float result = 0.0f;

		for (int i = 0; i < m_count; i++)
		{
			result += m_axes[i]->getAmount();
		}

		return Maths::Clamp(result, -1.0f, 1.0f);
	}
}
