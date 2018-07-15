#include "Constraint3.hpp"

#include <cassert>

namespace fl
{
	const Constraint3 Constraint3::ZERO = Constraint3(false, false, false);
	const Constraint3 Constraint3::ONE = Constraint3(true, true, true);

	Constraint3::Constraint3() :
		m_x(false),
		m_y(false),
		m_z(false)
	{
	}

	Constraint3::Constraint3(const Constraint3 &source) :
		m_x(source.m_x),
		m_y(source.m_y),
		m_z(source.m_z)
	{
	}

	Constraint3::Constraint3(const bool &x, const bool &y, const bool &z) :
		m_x(x),
		m_y(y),
		m_z(z)
	{
	}

	Constraint3::~Constraint3()
	{
	}

	Constraint3 Constraint3::Negate() const
	{
		return Constraint3(!m_x, !m_y, !m_z);
	}

	void Constraint3::Write(LoadedValue *destination)
	{
		destination->SetChild<bool>("x", m_x);
		destination->SetChild<bool>("y", m_y);
		destination->SetChild<bool>("z", m_z);
	}

	Constraint3 &Constraint3::operator=(const Constraint3 &other)
	{
		m_x = other.m_x;
		m_y = other.m_y;
		m_z = other.m_z;
		return *this;
	}

	Constraint3 &Constraint3::operator=(LoadedValue *source)
	{
		m_x = source->GetChild("x")->Get<bool>();
		m_y = source->GetChild("y")->Get<bool>();
		m_z = source->GetChild("z")->Get<bool>();
		return *this;
	}

	bool Constraint3::operator==(const Constraint3 &other) const
	{
		return m_x == other.m_x && m_y == other.m_x && m_z == other.m_z;
	}

	bool Constraint3::operator!=(const Constraint3 &other) const
	{
		return !(*this == other);
	}

	std::ostream &operator<<(std::ostream &stream, const Constraint3 &vector)
	{
		stream << vector.ToString();
		return stream;
	}

	Constraint3 Constraint3::operator-() const
	{
		return Negate();
	}

	const bool &Constraint3::operator[](const uint32_t &index) const
	{
		assert(index < 3);
		return m_elements[index];
	}

	bool &Constraint3::operator[](const uint32_t &index)
	{
		assert(index < 3);
		return m_elements[index];
	}

	std::string Constraint3::ToString() const
	{
		std::stringstream result;
		result << "Constraint3(" << m_x << ", " << m_y << ", " << m_z << ")";
		return result.str();
	}
}
