#include "Colour.hpp"

namespace acid
{
const Colour Colour::Clear = Colour("#000000", 0.0f);
const Colour Colour::Black = Colour("#000000");
const Colour Colour::Grey = Colour("#808080");
const Colour Colour::Silver = Colour("#C0C0C0");
const Colour Colour::White = Colour("#FFFFFF");
const Colour Colour::Maroon = Colour("#800000");
const Colour Colour::Red = Colour("#FF0000");
const Colour Colour::Olive = Colour("#808000");
const Colour Colour::Yellow = Colour("#FFFF00");
const Colour Colour::Green = Colour("#00FF00");
const Colour Colour::Lime = Colour("#008000");
const Colour Colour::Teal = Colour("#008080");
const Colour Colour::Aqua = Colour("#00FFFF");
const Colour Colour::Navy = Colour("#000080");
const Colour Colour::Blue = Colour("#0000FF");
const Colour Colour::Purple = Colour("#800080");
const Colour Colour::Fuchsia = Colour("#FF00FF");

Colour::Colour() :
	m_r(0.0f),
	m_g(0.0f),
	m_b(0.0f),
	m_a(1.0f)
{
}

Colour::Colour(const float &r, const float &g, const float &b, const float &a) :
	m_r(r),
	m_g(g),
	m_b(b),
	m_a(a)
{
}

Colour::Colour(const std::string &hex, const float &a) :
	m_r(0.0f),
	m_g(0.0f),
	m_b(0.0f),
	m_a(a)
{
	auto hexValue = std::stoul(hex.substr(1, hex.size()), nullptr, 16);

	m_r = static_cast<float>((hexValue >> 16) & 255) / 255.0f;
	m_g = static_cast<float>((hexValue >> 8) & 255) / 255.0f;
	m_b = static_cast<float>(hexValue & 255) / 255.0f;
}

Colour Colour::Add(const Colour &other) const
{
	return Colour(m_r + other.m_r, m_g + other.m_g, m_b + other.m_b, m_a + other.m_a);
}

Colour Colour::Subtract(const Colour &other) const
{
	return Colour(m_r - other.m_r, m_g - other.m_g, m_b - other.m_b, m_a - other.m_a);
}

Colour Colour::Multiply(const Colour &other) const
{
	return Colour(m_r * other.m_r, m_g * other.m_g, m_b * other.m_b, m_a * other.m_a);
}

Colour Colour::Divide(const Colour &other) const
{
	return Colour(m_r / other.m_r, m_g / other.m_g, m_b / other.m_b, m_a / other.m_a);
}

Colour Colour::Scale(const float &scalar) const
{
	return Colour(m_r * scalar, m_g * scalar, m_b * scalar, m_a * scalar);
}

Colour Colour::Lerp(const Colour &other, const float &progression) const
{
	Colour ta = *this * (1.0f - progression);
	Colour tb = other * progression;
	return ta + tb;
}

Colour Colour::Normalize() const
{
	float l = Length();

	if (l == 0.0f)
	{
		throw std::runtime_error("Can't normalize a zero length vector");
	}

	return Colour(m_r / l, m_g / l, m_b / l, m_a / l);
}

float Colour::LengthSquared() const
{
	return m_r * m_r + m_g * m_g + m_b * m_b + m_a * m_a;
}

float Colour::Length() const
{
	return std::sqrt(LengthSquared());
}

Colour Colour::SmoothDamp(const Colour &target, const Colour &rate) const
{
	return Colour(Maths::SmoothDamp(m_r, target.m_r, rate.m_r), Maths::SmoothDamp(m_g, target.m_g, rate.m_g), Maths::SmoothDamp(m_b, target.m_b, rate.m_b),
		Maths::SmoothDamp(m_a, target.m_a, rate.m_a));
}

Colour Colour::GetUnit() const
{
	float l = Length();
	return Colour(m_r / l, m_g / l, m_b / l, m_a / l);
}

std::string Colour::GetHex() const
{
	std::stringstream result;
	result << "#";

	char r[255];
	snprintf(r, 255, "%.2X", static_cast<int32_t>(m_r * 255.0f));
	result << r;

	char g[255];
	snprintf(g, 255, "%.2X", static_cast<int32_t>(m_g * 255.0f));
	result << g;

	char b[255];
	snprintf(b, 255, "%.2X", static_cast<int32_t>(m_b * 255.0f));
	result << b;

	return result.str();
}

std::string Colour::ToString() const
{
	std::stringstream stream;
	stream.precision(10);
	stream << "Colour(" << m_r << ", " << m_g << ", " << m_b << ", " << m_a << ")";
	return stream.str();
}

bool Colour::operator==(const Colour &other) const
{
	return m_r == other.m_r && m_g == other.m_g && m_b == other.m_b && m_a == other.m_a;
}

bool Colour::operator!=(const Colour &other) const
{
	return !(*this == other);
}

const float &Colour::operator[](const uint32_t &index) const
{
	switch (index)
	{
	case 0:
		return m_r;
	case 1:
		return m_g;
	case 2:
		return m_b;
	case 3:
		return m_a;
	default:
		throw std::runtime_error("Colour index out of bounds!");
	}
}

float &Colour::operator[](const uint32_t &index)
{
	switch (index)
	{
	case 0:
		return m_r;
	case 1:
		return m_g;
	case 2:
		return m_b;
	case 3:
		return m_a;
	default:
		throw std::runtime_error("Colour index out of bounds!");
	}
}

Colour operator+(const Colour &left, const Colour &right)
{
	return left.Add(right);
}

Colour operator-(const Colour &left, const Colour &right)
{
	return left.Subtract(right);
}

Colour operator*(const Colour &left, const Colour &right)
{
	return left.Multiply(right);
}

Colour operator/(const Colour &left, const Colour &right)
{
	return left.Divide(right);
}

Colour operator+(const float &value, const Colour &left)
{
	return Colour(value, value, value, 0.0f).Add(left);
}

Colour operator-(const float &value, const Colour &left)
{
	return Colour(value, value, value, 0.0f).Subtract(left);
}

Colour operator*(const float &value, const Colour &left)
{
	return Colour(value, value, value).Multiply(left);
}

Colour operator/(const float &value, const Colour &left)
{
	return Colour(value, value, value).Divide(left);
}

Colour operator+(const Colour &left, const float &value)
{
	return left.Add(Colour(value, value, value, 0.0f));
}

Colour operator-(const Colour &left, const float &value)
{
	return left.Subtract(Colour(value, value, value, 0.0f));
}

Colour operator*(const Colour &left, const float &value)
{
	return left.Multiply(Colour(value, value, value));
}

Colour operator/(const Colour &left, const float &value)
{
	return left.Divide(Colour(value, value, value));
}

Colour &Colour::operator+=(const Colour &other)
{
	return *this = Add(other);
}

Colour &Colour::operator-=(const Colour &other)
{
	return *this = Subtract(other);
}

Colour &Colour::operator*=(const Colour &other)
{
	return *this = Multiply(other);
}

Colour &Colour::operator/=(const Colour &other)
{
	return *this = Divide(other);
}

Colour &Colour::operator+=(const float &value)
{
	return *this = Add(Colour(value, value, value, 0.0f));
}

Colour &Colour::operator-=(const float &value)
{
	return *this = Subtract(Colour(value, value, value, 0.0f));
}

Colour &Colour::operator*=(const float &value)
{
	return *this = Multiply(Colour(value, value, value));
}

Colour &Colour::operator/=(const float &value)
{
	return *this = Divide(Colour(value, value, value));
}

const Metadata &operator>>(const Metadata &metadata, Colour &colour)
{
	std::string hex;
	metadata >> hex;
	colour = hex;
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Colour &colour)
{
	metadata << colour.GetHex();
	return metadata;
}

std::ostream &operator<<(std::ostream &stream, const Colour &colour)
{
	stream << colour.ToString();
	return stream;
}
}
