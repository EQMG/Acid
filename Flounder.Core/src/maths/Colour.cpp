#include "Colour.hpp"


#include "Maths.hpp"

#include "Vector3.hpp"

#include "Vector4.hpp"


namespace Flounder

{

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


		float r = ((1.0f - blend) * left.m_r) + (blend * right.m_r);

		float g = ((1.0f - blend) * left.m_g) + (blend * right.m_g);

		float b = ((1.0f - blend) * left.m_b) + (blend * right.m_b);

		float a = ((1.0f - blend) * left.m_a) + (blend * right.m_a);

		return destination->Set(r, g, b, a);

	}


	Colour *Colour::GetUnit(const Colour &source, Colour *destination)

	{

		if (destination == nullptr)

		{

			destination = new Colour();

		}


		float l = Length(source);

		return destination->Set(source)->Scale(1.0f / l);

	}


	std::string Colour::GetHex(const Colour &source)

	{

		std::string result = "#";


		char r[255];

		sprintf_s(r, "%.2X", static_cast<int>(source.m_r * 255.0f));

		result.append(r);


		char g[255];

		sprintf_s(g, "%.2X", static_cast<int>(source.m_g * 255.0f));

		result.append(g);


		char b[255];

		sprintf_s(b, "%.2X", static_cast<int>(source.m_b * 255.0f));

		result.append(b);


		return result;

	}


	float Colour::Length(const Colour &source)

	{

		return sqrt(LengthSquared(source));

	}


	float Colour::LengthSquared(const Colour &source)

	{

		return pow(source.m_r, 2) + pow(source.m_g, 2) + pow(source.m_b, 2) + pow(source.m_a, 2);

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
