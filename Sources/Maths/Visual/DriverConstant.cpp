#include "DriverConstant.hpp"

namespace acid
{
	DriverConstant::DriverConstant(const float &constant) :
		IDriver(Time::POSITIVE_INFINITY),
		m_constant(constant)
	{
	}

	float DriverConstant::Calculate(const float &factor)
	{
		return m_constant;
	}
}
