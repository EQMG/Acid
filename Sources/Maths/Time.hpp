#pragma once

#include <chrono>
#include <string>

#include "Export.hpp"

namespace acid {
using namespace std::chrono_literals;

class Node;

/**
 * @brief Represents a time value stored in microseconds.
 */
class ACID_EXPORT Time {
public:
	Time() = default;

	/*
	 * Creates a new time. This function is internal. To construct time values, use {@link Time::Seconds}, {@link Time::Milliseconds} or {@link Time::Microseconds} instead.
	 * @param duration The duration.
	 */
	template<typename Rep, typename Period>
	constexpr Time(const std::chrono::duration<Rep, Period> &duration);

	/**
	 * Creates a time value from a number of seconds.
	 * @tparam Rep The type of value to be casted to.
	 * @param seconds Number of seconds.
	 * @return Time value constructed from the amount of seconds.
	 */
	template<typename Rep = float>
	static constexpr Time Seconds(const Rep &seconds);

	/**
	 * Creates a time value from a number of milliseconds
	 * @tparam Rep The type of value to be casted to.
	 * @param milliseconds Number of milliseconds.
	 * @return Time value constructed from the amount of milliseconds.
	 */
	template<typename Rep = int32_t>
	static constexpr Time Milliseconds(const Rep &milliseconds);

	/**
	 * Creates a time value from a number of microseconds.
	 * @tparam Rep The type of value to be casted to.
	 * @param microseconds Number of microseconds.
	 * @return Time value constructed from the amount of microseconds.
	 */
	template<typename Rep = int64_t>
	static constexpr Time Microseconds(const Rep &microseconds);

	/**
	 * Gets the time value as a number of seconds.
	 * @tparam T The type of value to be casted to.
	 * @return Time in seconds.
	 */
	template<typename T = float>
	constexpr auto AsSeconds() const;

	/**
	 * Gets the time value as a number of milliseconds.
	 * @tparam T The type of value to be casted to.
	 * @return Time in milliseconds.
	 */
	template<typename T = int32_t>
	constexpr auto AsMilliseconds() const;

	/**
	 * Gets the time value as a number of microseconds.
	 * @tparam T The type of value to be casted to.
	 * @return Time in microseconds.
	 */
	template<typename T = int64_t>
	constexpr auto AsMicroseconds() const;

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

	//template<typename Period = std::ratio<1, 1>>
	//float Mod(const Time &other);

	template<typename Rep, typename Period>
	constexpr operator std::chrono::duration<Rep, Period>() const;

	constexpr bool operator==(const Time &other) const;
	constexpr bool operator!=(const Time &other) const;
	constexpr bool operator<(const Time &other) const;
	constexpr bool operator<=(const Time &other) const;
	constexpr bool operator>(const Time &other) const;
	constexpr bool operator>=(const Time &other) const;

	constexpr Time operator-() const;

	friend constexpr Time operator+(const Time &left, const Time &right);
	friend constexpr Time operator-(const Time &left, const Time &right);
	friend constexpr Time operator*(const Time &left, float right);
	friend constexpr Time operator*(const Time &left, int64_t right);
	friend constexpr Time operator*(float left, const Time &right);
	friend constexpr Time operator*(int64_t left, const Time &right);
	friend constexpr Time operator/(const Time &left, float right);
	friend constexpr Time operator/(const Time &left, int64_t right);
	friend constexpr double operator/(const Time &left, const Time &right);

	constexpr Time &operator+=(const Time &other);
	constexpr Time &operator-=(const Time &other);
	constexpr Time &operator*=(float other);
	constexpr Time &operator*=(int64_t other);
	constexpr Time &operator/=(float other);
	constexpr Time &operator/=(int64_t other);

	friend const Node &operator>>(const Node &node, Time &time);
	friend Node &operator<<(Node &node, const Time &time);

private:
	std::chrono::microseconds value{};
};
}

#include "Time.inl"
