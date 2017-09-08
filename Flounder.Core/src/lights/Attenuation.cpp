#include "attenuation.hpp"

namespace Flounder
{
	Attenuation::Attenuation(const float &constant, const float &linear, const float &exponent)
	{
		Set(constant, linear, exponent);
	}

	Attenuation::Attenuation(Attenuation *source)
	{
		Set(source);
	}

	Attenuation::~Attenuation()
	{
	}

	Attenuation Attenuation::Set(const float &constant, const float &linear, const float &exponent)
	{
		m_constant = constant;
		m_linear = linear;
		m_exponent = exponent;
		return this;
	}

	Attenuation Attenuation::Set(Attenuation *source)
	{
		m_constant = source->m_constant;
		m_linear = source->m_linear;
		m_exponent = source->m_exponent;
		return this;
	}
}
