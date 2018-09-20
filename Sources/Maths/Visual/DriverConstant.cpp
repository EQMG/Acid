#include "DriverConstant.hpp"

namespace acid
{
	DriverConstant::DriverConstant(const float &constant) :
		IDriver(+std::numeric_limits<float>::infinity()),
		m_constant(constant)
	{
	}

	float DriverConstant::Calculate(const float &time)
	{
		return m_constant;
	}
}
