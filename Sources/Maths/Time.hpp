#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
/**
 * @brief Represents a time value stored in microseconds.
 */
class ACID_EXPORT Time
{
public:
	/**
	 * Creates a new time. This function is internal. To construct time values, use Time::Seconds, Time::Milliseconds or Time::Microseconds instead.
	 * @param microseconds Number of microseconds.
	 */
	explicit Time(const int64_t &microseconds = 0);

	/**
	 * Creates a time value from a number of seconds.
	 * @tparam T The type of value to be casted to.
	 * @param amount Number of seconds.
	 * @return Time value constructed from the amount of seconds.
	 */
	template<typename T = float>
	static Time Seconds(const T &amount)
	{
		return Time(static_cast<int64_t>(amount * static_cast<T>(1000000)));
	}

	/**
	 * Creates a time value from a number of milliseconds
	 * @tparam T The type of value to be casted to.
	 * @param amount Number of milliseconds.
	 * @return Time value constructed from the amount of milliseconds.
	 */
	template<typename T = int32_t>
	static Time Milliseconds(const T &amount)
	{
		return Time(static_cast<int64_t>(amount * static_cast<T>(1000)));
	}

	/**
	 * Creates a time value from a number of microseconds.
	 * @tparam T The type of value to be casted to.
	 * @param amount Number of microseconds.
	 * @return Time value constructed from the amount of microseconds.
	 */
	template<typename T = int64_t>
	static Time Microseconds(const T &amount)
	{
		return Time(static_cast<int64_t>(amount));
	}

	/**
	 * Gets the time value as a number of seconds.
	 * @tparam T The type of value to be casted to.
	 * @return Time in seconds.
	 */
	template<typename T = float>
	auto AsSeconds() const
	{
		return static_cast<T>(m_microseconds) / static_cast<T>(1000000);
	}

	/**
	 * Gets the time value as a number of milliseconds.
	 * @tparam T The type of value to be casted to.
	 * @return Time in milliseconds.
	 */
	template<typename T = int32_t>
	auto AsMilliseconds() const
	{
		return static_cast<T>(m_microseconds) / static_cast<T>(1000);
	}

	/**
	 * Gets the time value as a number of microseconds.
	 * @tparam T The type of value to be casted to.
	 * @return Time in microseconds.
	 */
	template<typename T = int64_t>
	auto AsMicroseconds() const
	{
		return static_cast<T>(m_microseconds);
	}

	bool operator==(const Time &other) const;

	bool operator!=(const Time &other) const;

	bool operator<(const Time &other) const;

	bool operator<=(const Time &other) const;

	bool operator>(const Time &other) const;

	bool operator>=(const Time &other) const;

	Time operator-() const;

	ACID_EXPORT friend Time operator+(const Time &left, const Time &right);

	ACID_EXPORT friend Time operator-(const Time &left, const Time &right);

	ACID_EXPORT friend Time operator*(const Time &left, const float &right);

	ACID_EXPORT friend Time operator*(const Time &left, const int64_t &right);

	ACID_EXPORT friend Time operator*(const float &left, const Time &right);

	ACID_EXPORT friend Time operator*(const int64_t &left, const Time &right);

	ACID_EXPORT friend Time operator/(const Time &left, const float &right);

	ACID_EXPORT friend Time operator/(const Time &left, const int64_t &right);

	ACID_EXPORT friend float operator/(const Time &left, const Time &right);

	ACID_EXPORT friend Time operator%(const Time &left, const Time &right);

	Time &operator+=(const Time &other);

	Time &operator-=(const Time &other);

	Time &operator*=(const float &other);

	Time &operator*=(const int64_t &other);

	Time &operator/=(const float &other);

	Time &operator/=(const int64_t &other);

	Time &operator%=(const Time &other);

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, Time &time);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const Time &time);

	static const Time Zero;
	static const Time Min;
	static const Time Max;
private:
	int64_t m_microseconds;
};
}
