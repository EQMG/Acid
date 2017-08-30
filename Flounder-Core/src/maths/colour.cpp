#include "colour.hpp"

#include "vector3.hpp"
#include "vector4.hpp"

namespace flounder
{
	colour::colour() :
		m_r(0.0f),
		m_g(0.0f),
		m_b(0.0f),
		m_a(1.0f)
	{
	}

	colour::colour(const float &r, const float &g, const float &b, const float &a) :
		m_r(r),
		m_g(g),
		m_b(b),
		m_a(a)
	{
	}

	colour::colour(const std::string &hex, const float &a) :
		m_r(0.0f),
		m_g(0.0f),
		m_b(0.0f),
		m_a(a)
	{
		int r, g, b;
		sscanf(hex.c_str() + (hex[0] == '#' ? 1 : 0), "%2x%2x%2x", &r, &g, &b);
		m_r = static_cast<float>(r) / 255.0f;
		m_g = static_cast<float>(g) / 255.0f;
		m_b = static_cast<float>(b) / 255.0f;
	}

	colour::colour(const colour &source) :
		m_r(source.m_r),
		m_g(source.m_g),
		m_b(source.m_b),
		m_a(source.m_a)
	{
	}

	colour::colour(const vector3 &source) :
		m_r(source.m_x),
		m_g(source.m_y),
		m_b(source.m_z),
		m_a(1.0f)
	{
	}

	colour::colour(const vector4 &source) :
		m_r(source.m_x),
		m_g(source.m_y),
		m_b(source.m_z),
		m_a(source.m_w)
	{
	}

	colour::~colour()
	{
	}

	colour *colour::set(const float &r, const float &g, const float &b, const float &a)
	{
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
		return this;
	}

	colour *colour::set(const std::string &hex, const float &a)
	{
		int r, g, b;
		sscanf(hex.c_str() + (hex[0] == '#' ? 1 : 0), "%2x%2x%2x", &r, &g, &b);
		m_r = static_cast<float>(r) / 255.0f;
		m_g = static_cast<float>(g) / 255.0f;
		m_b = static_cast<float>(b) / 255.0f;
		m_a = a;
		return this;
	}

	colour *colour::set(const colour &source)
	{
		m_r = source.m_r;
		m_g = source.m_g;
		m_b = source.m_b;
		m_a = source.m_a;
		return this;
	}

	colour *colour::set(const vector3 &source)
	{
		m_r = source.m_x;
		m_g = source.m_y;
		m_b = source.m_z;
		m_a = 1.0f;
		return this;
	}

	colour *colour::set(const vector4 &source)
	{
		m_r = source.m_x;
		m_g = source.m_y;
		m_b = source.m_z;
		m_a = source.m_w;
		return this;
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

	std::string colour::getHex()
	{
		std::string result = "#";

		char r[255];
		sprintf_s(r, "%.2X", static_cast<int>(m_r * 255.0f));
		result.append(r);

		char g[255];
		sprintf_s(g, "%.2X", static_cast<int>(m_g * 255.0f));
		result.append(g);

		char b[255];
		sprintf_s(b, "%.2X", static_cast<int>(m_b * 255.0f));
		result.append(b);

		return result;
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
