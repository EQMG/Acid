#include "DriverConstant.hpp"

namespace acid
{
	DriverConstant::DriverConstant(const float &constant) :
		IDriver(Time::PositiveInfinity),
		m_constant(constant)
	{
	}

	float DriverConstant::Calculate(const float &factor)
	{
		return m_constant;
	}
}
