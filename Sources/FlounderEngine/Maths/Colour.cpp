#include "Colour.hpp"

#include <sstream>
#include "Maths.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace Flounder
{
	const Colour Colour::CLEAR = Colour(0.0f, 0.0f, 0.0f, 0.0f);
	const Colour Colour::WHITE = Colour("#ffffff");
	const Colour Colour::BLACK = Colour("#000000");

	Colour::Colour() :
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}

	Colour::Colour(const float &r, const float &g, const float &b, const float &a) :
		Vector4(r, g, b, a)
	{
	}

	Colour::Colour(const std::string &hex, const float &a) :
		Vector4(0.0f, 0.0f, 0.0f, a)
	{
		*this = hex;
	}

	Colour::Colour(const Colour &source) :
		Vector4(source)
	{
	}

	Colour::Colour(const Vector3 &source) :
		Vector4(source.m_x, source.m_y, source.m_z, 1.0f)
	{
	}

	Colour::Colour(const Vector4 &source) :
		Vector4(source.m_x, source.m_y, source.m_z, source.m_w)
	{
	}

	Colour::~Colour()
	{
	}

	Colour Colour::Interpolate(const Colour &other, const float blend) const
	{
		float r = Maths::Interpolate(m_r, other.m_r, blend);
		float g = Maths::Interpolate(m_g, other.m_g, blend);
		float b = Maths::Interpolate(m_b, other.m_b, blend);
		float a = Maths::Interpolate(m_a, other.m_a, blend);
		return Colour(r, g, b, a);
	}

	Colour Colour::GetUnit()
	{
		float l = Length();
		return Colour(m_r / l, m_g / l, m_b / l, m_a / l);
	}

	std::string Colour::GetHex()
	{
		std::string result = "#";

		char r[255];
		snprintf(r, 255, "%.2X", static_cast<int>(m_r * 255.0f));
		result.append(r);

		char g[255];
		snprintf(g, 255, "%.2X", static_cast<int>(m_g * 255.0f));
		result.append(g);

		char b[255];
		snprintf(b, 255, "%.2X", static_cast<int>(m_b * 255.0f));
		result.append(b);

		return result;
	}

	void Colour::Write(LoadedValue *destination)
	{
		destination->SetChild<float>("r", m_r);
		destination->SetChild<float>("g", m_g);
		destination->SetChild<float>("b", m_b);
		destination->SetChild<float>("a", m_a);
	}

	Colour &Colour::operator=(const std::string &hex)
	{
		int r, g, b;
		sscanf(hex.c_str() + (hex[0] == '#' ? 1 : 0), "%2x%2x%2x", &r, &g, &b);
		m_r = static_cast<float>(r) / 255.0f;
		m_g = static_cast<float>(g) / 255.0f;
		m_b = static_cast<float>(b) / 255.0f;
		return *this;
	}

	Colour &Colour::operator=(LoadedValue *source)
	{
		m_r = source->GetChild("r")->Get<float>();
		m_g = source->GetChild("g")->Get<float>();
		m_b = source->GetChild("b")->Get<float>();
		m_a = source->GetChild("a")->Get<float>();
		return *this;
	}

	std::ostream &operator<<(std::ostream &stream, const Colour &vector)
	{
		stream << vector.ToString();
		return stream;
	}

	std::string Colour::ToString() const
	{
		std::stringstream result;
		result << "Colour(" << m_r << ", " << m_g << ", " << m_b << ", " << m_a << ")";
		return result.str();
	}
}
