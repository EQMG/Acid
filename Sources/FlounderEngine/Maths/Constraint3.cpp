#include "Constraint3.hpp"

namespace Flounder
{
	const Constraint3 Constraint3::FALSE = Constraint3(false, false, false);
	const Constraint3 Constraint3::TRUE = Constraint3(true, true, true);

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

	Constraint3::Constraint3(LoadedValue *value)
	{
		Set(value);
	}

	Constraint3::~Constraint3()
	{
	}

	Constraint3 *Constraint3::Set(const Constraint3 &source)
	{
		m_x = source.m_x;
		m_y = source.m_y;
		m_z = source.m_z;
		return this;
	}

	Constraint3 *Constraint3::Set(const bool &x, const bool &y, const bool &z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
		return this;
	}

	Constraint3 *Constraint3::Set(LoadedValue *value)
	{
		m_x = (bool) value->GetChild("x")->Get<int>();
		m_y = (bool) value->GetChild("y")->Get<int>();
		m_z = (bool) value->GetChild("z")->Get<int>();
		return this;
	}

	void Constraint3::Write(LoadedValue *destination)
	{
		destination->SetChild<int>("x", (int) m_x);
		destination->SetChild<int>("y", (int) m_y);
		destination->SetChild<int>("z", (int) m_z);
	}

	Constraint3 &Constraint3::operator=(const Constraint3 &other)
	{
		return *Set(other);
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

	std::string Constraint3::ToString() const
	{
		std::stringstream result;
		result << "Constraint3(" << m_x << ", " << m_y << ", " << m_z << ")";
		return result.str();
	}
}
