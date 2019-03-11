#include "Time.hpp"

namespace acid
{
	const Time Time::Zero = Time();
	const Time Time::PositiveInfinity = Time(+std::numeric_limits<int64_t>::infinity());
	const Time Time::NegativeInfinity = Time(-std::numeric_limits<int64_t>::infinity());

	Time::Time(const int64_t &microseconds) :
		m_microseconds(microseconds)
	{
	}

	Time Time::Seconds(const float &amount)
	{
		return Time(static_cast<int64_t>(amount * 1000000));
	}

	Time Time::Milliseconds(const int32_t &amount)
	{
		return Time(static_cast<int64_t>(amount) * 1000);
	}

	Time Time::Microseconds(const int64_t &amount)
	{
		return Time(amount);
	}

	float Time::AsSeconds() const
	{
		return static_cast<float>(m_microseconds) / 1000000.0f;
	}

	int32_t Time::AsMilliseconds() const
	{
		return static_cast<int32_t>(m_microseconds / 1000);
	}

	int64_t Time::AsMicroseconds() const
	{
		return m_microseconds;
	}

	bool Time::operator==(const Time &other) const
	{
		return AsMicroseconds() == other.AsMicroseconds();
	}

	bool Time::operator!=(const Time &other) const
	{
		return AsMicroseconds() != other.AsMicroseconds();
	}

	bool Time::operator<(const Time &other) const
	{
		return AsMicroseconds() < other.AsMicroseconds();
	}

	bool Time::operator<=(const Time &other) const
	{
		return AsMicroseconds() <= other.AsMicroseconds();
	}

	bool Time::operator>(const Time &other) const
	{
		return AsMicroseconds() > other.AsMicroseconds();
	}

	bool Time::operator>=(const Time &other) const
	{
		return AsMicroseconds() >= other.AsMicroseconds();
	}

	Time Time::operator-() const
	{
		return Time(-AsMicroseconds());
	}

	Time operator+(const Time &left, const Time &right)
	{
		return Time::Microseconds(left.AsMicroseconds() + right.AsMicroseconds());
	}

	Time operator-(const Time &left, const Time &right)
	{
		return Time::Microseconds(left.AsMicroseconds() - right.AsMicroseconds());
	}

	Time operator*(const Time &left, const float &right)
	{
		return Time::Seconds(left.AsSeconds() * right);
	}

	Time operator*(const Time &left, const int64_t &right)
	{
		return Time::Microseconds(left.AsMicroseconds() * right);
	}

	Time operator*(const float &left, const Time &right)
	{
		return right * left;
	}

	Time operator*(const int64_t &left, const Time &right)
	{
		return right * left;
	}

	Time operator/(const Time &left, const float &right)
	{
		return Time::Seconds(left.AsSeconds() / right);
	}

	Time operator/(const Time &left, const int64_t &right)
	{
		return Time::Microseconds(left.AsMicroseconds() / right);
	}

	float operator/(const Time &left, const Time &right)
	{
		return left.AsSeconds() / right.AsSeconds();
	}

	Time operator%(const Time &left, const Time &right)
	{
		return Time::Microseconds(std::fmod(left.AsMicroseconds(), right.AsMicroseconds()));
	}

	Time &Time::operator+=(const Time &other)
	{
		return *this = *this + other;
	}

	Time &Time::operator-=(const Time &other)
	{
		return *this = *this - other;
	}

	Time &Time::operator*=(const float &other)
	{
		return *this = *this * other;
	}

	Time &Time::operator*=(const int64_t &other)
	{
		return *this = *this * other;
	}

	Time &Time::operator/=(const float &other)
	{
		return *this = *this / other;
	}

	Time &Time::operator/=(const int64_t &other)
	{
		return *this = *this / other;
	}

	Time &Time::operator%=(const Time &other)
	{
		return *this = *this % other;
	}
}
