#include "driverconstant.h"

namespace flounder
{
	driverconstant::driverconstant(const float &constant)
		: idriver(1.0f)
	{
		m_value = constant;
	}

	driverconstant::~driverconstant()
	{
	}

	float driverconstant::calculate(const float &time)
	{
		return m_value;
	}
}
