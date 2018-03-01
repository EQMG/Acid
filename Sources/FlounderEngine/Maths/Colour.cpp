#include "Colour.hpp"

#include "Maths.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace Flounder
{
	const Colour Colour::CLEAR = Colour(0.0f, 0.0f, 0.0f, 0.0f);
	const Colour Colour::WHITE = Colour("#ffffff");
	const Colour Colour::BLACK = Colour("#000000");

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
		int r, g, b;
		sscanf(hex.c_str() + (hex[0] == '#' ? 1 : 0), "%2x%2x%2x", &r, &g, &b);
		m_r = static_cast<float>(r) / 255.0f;
		m_g = static_cast<float>(g) / 255.0f;
		m_b = static_cast<float>(b) / 255.0f;
	}

	Colour::Colour(const Colour &source) :
		m_r(source.m_r),
		m_g(source.m_g),
		m_b(source.m_b),
		m_a(source.m_a)
	{
	}

	Colour::Colour(const Vector3 &source) :
		m_r(source.m_x),
		m_g(source.m_y),
		m_b(source.m_z),
		m_a(1.0f)
	{
	}

	Colour::Colour(const Vector4 &source) :
		m_r(source.m_x),
		m_g(source.m_y),
		m_b(source.m_z),
		m_a(source.m_w)
	{
	}

	Colour::Colour(LoadedValue *value)
	{
		Set(value);
	}

	Colour::~Colour()
	{
	}

	Colour *Colour::Set(const float &r, const float &g, const float &b, const float &a)
	{
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
		return this;
	}

	Colour *Colour::Set(const std::string &hex, const float &a)
	{
		int r, g, b;
		sscanf(hex.c_str() + (hex[0] == '#' ? 1 : 0), "%2x%2x%2x", &r, &g, &b);
		m_r = static_cast<float>(r) / 255.0f;
		m_g = static_cast<float>(g) / 255.0f;
		m_b = static_cast<float>(b) / 255.0f;
		m_a = a;
		return this;
	}

	Colour *Colour::Set(const Colour &source)
	{
		m_r = source.m_r;
		m_g = source.m_g;
		m_b = source.m_b;
		m_a = source.m_a;
		return this;
	}

	Colour *Colour::Set(const Vector3 &source)
	{
		m_r = source.m_x;
		m_g = source.m_y;
		m_b = source.m_z;
		m_a = 1.0f;
		return this;
	}

	Colour *Colour::Set(const Vector4 &source)
	{
		m_r = source.m_x;
		m_g = source.m_y;
		m_b = source.m_z;
		m_a = source.m_w;
		return this;
	}

	Colour *Colour::Set(LoadedValue *value)
	{
		m_r = value->GetChild("r")->Get<float>();
		m_g = value->GetChild("g")->Get<float>();
		m_b = value->GetChild("b")->Get<float>();
		m_a = value->GetChild("a")->Get<float>();
		return this;
	}

	void Colour::Write(LoadedValue *destination)
	{
		destination->SetChild<float>("r", m_r);
		destination->SetChild<float>("g", m_g);
		destination->SetChild<float>("b", m_b);
		destination->SetChild<float>("a", m_a);
	}

	Colour *Colour::Add(const Colour &left, const Colour &right, Colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new Colour();
		}

		return destination->Set(left.m_r + right.m_r, left.m_g + right.m_g, left.m_b + right.m_b, left.m_a + right.m_a);
	}

	Colour *Colour::Subtract(const Colour &left, const Colour &right, Colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new Colour();
		}

		return destination->Set(left.m_r - right.m_r, left.m_g - right.m_g, left.m_b - right.m_b, left.m_a - right.m_a);
	}

	Colour *Colour::Multiply(const Colour &left, const Colour &right, Colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new Colour();
		}

		return destination->Set(left.m_r * right.m_r, left.m_g * right.m_g, left.m_b * right.m_b, left.m_a * right.m_a);
	}

	Colour *Colour::Divide(const Colour &left, const Colour &right, Colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new Colour();
		}

		return destination->Set(left.m_r / right.m_r, left.m_g / right.m_g, left.m_b / right.m_b, left.m_a / right.m_a);
	}

	Colour *Colour::Interpolate(const Colour &left, const Colour &right, const float blend, Colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new Colour();
		}

		const float r = Maths::Interpolate(left.m_r, right.m_r, blend);
		const float g = Maths::Interpolate(left.m_g, right.m_g, blend);
		const float b = Maths::Interpolate(left.m_b, right.m_b, blend);
		const float a = Maths::Interpolate(left.m_a, right.m_a, blend);
		return destination->Set(r, g, b, a);
	}

	Colour *Colour::GetUnit(const Colour &source, Colour *destination)
	{
		if (destination == nullptr)
		{
			destination = new Colour();
		}

		const float l = Length(source);
		return destination->Set(source)->Scale(1.0f / l);
	}

	std::string Colour::GetHex(const Colour &source)
	{
		std::string result = "#";

		char r[255];
		snprintf(r, 255, "%.2X", static_cast<int>(source.m_r * 255.0f));
		result.append(r);

		char g[255];
		snprintf(g, 255, "%.2X", static_cast<int>(source.m_g * 255.0f));
		result.append(g);

		char b[255];
		snprintf(b, 255, "%.2X", static_cast<int>(source.m_b * 255.0f));
		result.append(b);

		return result;
	}

	float Colour::Length(const Colour &source)
	{
		return std::sqrt(LengthSquared(source));
	}

	float Colour::LengthSquared(const Colour &source)
	{
		return std::pow(source.m_r, 2.0f) + std::pow(source.m_g, 2.0f) + std::pow(source.m_b, 2.0f) + std::pow(source.m_a, 2.0f);
	}

	Colour *Colour::Scale(const float &scalar)
	{
		m_r *= scalar;
		m_g *= scalar;
		m_b *= scalar;
		return this;
	}

	float Colour::Length()
	{
		return Length(*this);
	}

	float Colour::LengthSquared()
	{
		return LengthSquared(*this);
	}
}
