#include "DriverConstant.hpp"

namespace Flounder
{
	DriverConstant::DriverConstant(const float &constant) :
		IDriver(1.0f),
		m_value(constant)
	{
	}

	DriverConstant::~DriverConstant()
	{
	}

	float DriverConstant::Calculate(const float &time)
	{
		return m_value;
	}
}
