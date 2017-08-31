#include "attenuation.hpp"

namespace Flounder
{
	attenuation::attenuation(const float &constant, const float &linear, const float &exponent)
	{
		set(constant, linear, exponent);
	}

	attenuation::attenuation(attenuation *source)
	{
		set(source);
	}

	attenuation::~attenuation()
	{
	}

	attenuation attenuation::set(const float &constant, const float &linear, const float &exponent)
	{
		m_constant = constant;
		m_linear = linear;
		m_exponent = exponent;
		return this;
	}

	attenuation attenuation::set(attenuation *source)
	{
		m_constant = source->m_constant;
		m_linear = source->m_linear;
		m_exponent = source->m_exponent;
		return this;
	}
}
