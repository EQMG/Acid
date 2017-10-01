#include "Attenuation.hpp"

namespace Flounder
{
	Attenuation::Attenuation() :
		m_constant(1.0f),
		m_linear(0.0f),
		m_exponent(0.0f)
	{
	}

	Attenuation::Attenuation(const float &constant, const float &linear, const float &exponent) :
		m_constant(constant),
		m_linear(linear),
		m_exponent(exponent)
	{
		Set(constant, linear, exponent);
	}

	Attenuation::Attenuation(const Attenuation &source) :
		m_constant(source.m_constant),
		m_linear(source.m_linear),
		m_exponent(source.m_exponent)
	{
	}

	Attenuation::~Attenuation()
	{
	}

	Attenuation *Attenuation::Set(const float &constant, const float &linear, const float &exponent)
	{
		m_constant = constant;
		m_linear = linear;
		m_exponent = exponent;
		return this;
	}

	Attenuation *Attenuation::Set(const Attenuation &source)
	{
		m_constant = source.m_constant;
		m_linear = source.m_linear;
		m_exponent = source.m_exponent;
		return this;
	}
}
