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
	Time();

	/*
	 * Creates a new time. This function is internal. To construct time values, use {@link Time::Seconds}, {@link Time::Milliseconds} or {@link Time::Microseconds} instead.
	 * @param duration The duration.
	 */
	template<typename Rep, typename Period>
	constexpr Time(const std::chrono::duration<Rep, Period> &duration) :
		m_microseconds(std::chrono::duration_cast<std::chrono::microseconds>(duration).count())
	{
	}

	/**
	 * Creates a time value from a number of seconds.
	 * @tparam Rep The type of value to be casted to.
	 * @param seconds Number of seconds.
	 * @return Time value constructed from the amount of seconds.
	 */
	template<typename Rep = float>
	constexpr static Time Seconds(const Rep &seconds)
	{
		return Time(std::chrono::duration<Rep>(seconds));
	}

	/**
	 * Creates a time value from a number of milliseconds
	 * @tparam Rep The type of value to be casted to.
	 * @param milliseconds Number of milliseconds.
	 * @return Time value constructed from the amount of milliseconds.
	 */
	template<typename Rep = int32_t>
	constexpr static Time Milliseconds(const Rep &milliseconds)
	{
		return Time(std::chrono::duration<Rep, std::micro>(milliseconds));
	}

	/**
	 * Creates a time value from a number of microseconds.
	 * @tparam Rep The type of value to be casted to.
	 * @param microseconds Number of microseconds.
	 * @return Time value constructed from the amount of microseconds.
	 */
	template<typename Rep = int64_t>
	constexpr static Time Microseconds(const Rep &microseconds)
	{
		return Time(std::chrono::duration<Rep, std::micro>(microseconds));
	}

	template<typename Rep, typename Period>
	operator std::chrono::duration<Rep, Period>() const { return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(m_microseconds); }

	/**
	 * Gets the time value as a number of seconds.
	 * @tparam T The type of value to be casted to.
	 * @return Time in seconds.
	 */
	template<typename T = float>
	auto AsSeconds() const
	{
		return static_cast<T>(m_microseconds.count()) / static_cast<T>(1000000);
	}

	/**
	 * Gets the time value as a number of milliseconds.
	 * @tparam T The type of value to be casted to.
	 * @return Time in milliseconds.
	 */
	template<typename T = int32_t>
	auto AsMilliseconds() const
	{
		return static_cast<T>(m_microseconds.count()) / static_cast<T>(1000);
	}

	/**
	 * Gets the time value as a number of microseconds.
	 * @tparam T The type of value to be casted to.
	 * @return Time in microseconds.
	 */
	template<typename T = int64_t>
	auto AsMicroseconds() const
	{
		return static_cast<T>(m_microseconds.count());
	}

	/**
	 * Gets the current time of this application.
	 * @return The current time.
	 */
	static Time Now();

	/**
	 * Gets the current system time as a string. "%d-%m-%Y %I:%M:%S"
	 * @tparam format The format to put the time into.
	 * @return The date time as a string.
	 */
	static std::string GetDateTime(const std::string &format = "%Y-%m-%d %H:%M:%S");

	/*template<typename Period = std::ratio<1, 1>>
	float Mod(const Time &other)
	{
		return std::modf(std::chrono::duration_cast<std::chrono::duration<double, Period>>(m_microseconds),
			std::chrono::duration_cast<std::chrono::duration<double, Period>>(other.m_microseconds));
	}*/

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
	
	ACID_EXPORT friend double operator/(const Time &left, const Time &right);

	Time &operator+=(const Time &other);

	Time &operator-=(const Time &other);

	Time &operator*=(const float &other);

	Time &operator*=(const int64_t &other);

	Time &operator/=(const float &other);

	Time &operator/=(const int64_t &other);

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, Time &time);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const Time &time);

private:
	static const std::chrono::time_point<std::chrono::high_resolution_clock> Start;

	std::chrono::microseconds m_microseconds;
};
}
