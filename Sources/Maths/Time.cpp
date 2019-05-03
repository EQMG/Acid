#include "Time.hpp"

namespace acid
{
const Time Time::Zero = Time();
const Time Time::Min = Time(std::numeric_limits<int64_t>::min());
const Time Time::Max = Time(std::numeric_limits<int64_t>::max());

Time::Time(const int64_t &microseconds) :
	m_microseconds(microseconds)
{
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

const Metadata &operator>>(const Metadata &metadata, Time &time)
{
	metadata >> time.m_microseconds;
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Time &time)
{
	metadata << time.m_microseconds;
	return metadata;
}
}
