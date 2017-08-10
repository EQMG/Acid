#include "colour.h"

namespace flounder
{
	colour::colour()
	{
		set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	colour::colour(const float &r, const float &g, const float &b)
	{
		set(r, g, b);
	}

	colour::colour(const float &r, const float &g, const float &b, const bool &convert)
	{
		set(r, g, b, convert);
	}

	colour::colour(const float &r, const float &g, const float &b, const float &a)
	{
		set(r, g, b, a);
	}

	colour::colour(const float &r, const float &g, const float &b, const float &a, const bool &convert)
	{
		set(r, g, b, a, convert);
	}

	colour::colour(const colour &source)
	{
		set(source);
	}

	colour::~colour()
	{
	}

	colour *colour::set(const float &r, const float &g, const float &b)
	{
		return set(r, g, b, false);
	}

	colour *colour::set(const float &r, const float &g, const float &b, const bool &convert)
	{
		if (convert)
		{
			m_r = r / 255.0f;
			m_g = g / 255.0f;
			m_b = b / 255.0f;
		}
		else
		{
			m_r = r;
			m_g = g;
			m_b = b;
		}

		return this;
	}

	colour *colour::set(const float &r, const float &g, const float &b, const float &a)
	{
		return set(r, g, b, a, false);
	}

	colour *colour::set(const float &r, const float &g, const float &b, const float &a, const bool &convert)
	{
		if (convert)
		{
			m_r = r / 255.0f;
			m_g = g / 255.0f;
			m_b = b / 255.0f;
			m_a = a / 255.0f;
		}
		else
		{
			m_r = r;
			m_g = g;
			m_b = b;
			m_a = a;
		}

		return this;
	}

	colour *colour::set(const colour &source)
	{
		return set(source.m_r, source.m_g, source.m_b, source.m_a);
	}

	colour *colour::add(const colour &left, const colour &right, colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new colour();
		}

		return destination->set(left.m_r + right.m_r, left.m_g + right.m_g, left.m_b + right.m_b, left.m_a + right.m_a);
	}

	colour *colour::subtract(const colour &left, const colour &right, colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new colour();
		}

		return destination->set(left.m_r - right.m_r, left.m_g - right.m_g, left.m_b - right.m_b, left.m_a - right.m_a);
	}

	colour *colour::multiply(const colour &left, const colour &right, colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new colour();
		}

		return destination->set(left.m_r * right.m_r, left.m_g * right.m_g, left.m_b * right.m_b, left.m_a * right.m_a);
	}

	colour *colour::divide(const colour &left, const colour &right, colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new colour();
		}

		return destination->set(left.m_r / right.m_r, left.m_g / right.m_g, left.m_b / right.m_b, left.m_a / right.m_a);
	}

	colour *colour::interpolate(const colour &left, const colour &right, const float blend, colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new colour();
		}

		float r = ((1.0f - blend) * left.m_r) + (blend * right.m_r);
		float g = ((1.0f - blend) * left.m_g) + (blend * right.m_g);
		float b = ((1.0f - blend) * left.m_b) + (blend * right.m_b);
		float a = ((1.0f - blend) * left.m_a) + (blend * right.m_a);
		return destination->set(r, g, b, a);
	}

	colour *colour::getUnit(const colour &source, colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new colour();
		}

		float l = length(source);
		return destination->set(source)->scale(1.0f / l);
	}

	float colour::length(const colour &source)
	{
		return sqrt(lengthSquared(source));
	}

	float colour::lengthSquared(const colour &source)
	{
		return pow(source.m_r, 2) + pow(source.m_g, 2) + pow(source.m_b, 2) + pow(source.m_a, 2);
	}

	colour *colour::scale(const float scalar)
	{
		m_r *= scalar;
		m_g *= scalar;
		m_b *= scalar;
		return this;
	}

	float colour::length()
	{
		return length(*this);
	}

	float colour::lengthSquared()
	{
		return lengthSquared(*this);
	}
}
