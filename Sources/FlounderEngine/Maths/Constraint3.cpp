#include "Constraint3.hpp"

namespace fl
{
//	const Constraint3 Constraint3::FALSE = Constraint3(false, false, false);
//	const Constraint3 Constraint3::TRUE = Constraint3(true, true, true);

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

	void Constraint3::Write(LoadedValue *destination)
	{
		destination->SetChild<int>("x", static_cast<int>(m_x));
		destination->SetChild<int>("y", static_cast<int>(m_y));
		destination->SetChild<int>("z", static_cast<int>(m_z));
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
		m_x = static_cast<bool>(source->GetChild("x")->Get<int>());
		m_y = static_cast<bool>(source->GetChild("y")->Get<int>());
		m_z = static_cast<bool>(source->GetChild("z")->Get<int>());
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

	std::string Constraint3::ToString() const
	{
		std::stringstream result;
		result << "Constraint3(" << m_x << ", " << m_y << ", " << m_z << ")";
		return result.str();
	}
}
