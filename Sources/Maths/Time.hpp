#pragma once

#include <stdint.h>
#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// Represents a time value stored in microseconds.
	/// </summary>
	class ACID_EXPORT Time
	{
	private:
		int64_t m_microseconds;
	public:
		static const Time ZERO;
		static const Time NEGATIVE_INFINITY;
		static const Time POSITIVE_INFINITY;

		/// <summary>
		/// Constructor for time. This function is internal. To construct time values, use Time::Seconds, Time::Milliseconds or Time::Microseconds instead.
		/// </summary>
		/// <param name="microseconds"> Number of microseconds. </param>
		explicit Time(const int64_t &microseconds = 0);

		/// <summary>
		/// Constructs a time value from a number of seconds.
		/// </summary>
		/// <param name="amount"> Number of seconds. </param>
		/// <returns> Time value constructed from the amount of seconds. </returns>
		static Time Seconds(const float &amount);

		/// <summary>
		/// Constructs a time value from a number of milliseconds.
		/// </summary>
		/// <param name="amount"> Number of milliseconds. </param>
		/// <returns> Time value constructed from the amount of milliseconds. </returns>
		static Time Milliseconds(const int32_t &amount);

		/// <summary>
		/// Constructs a time value from a number of microseconds.
		/// </summary>
		/// <param name="amount"> Number of microseconds. </param>
		/// <returns> Time value constructed from the amount of microseconds. </returns>
		static Time Microseconds(const int64_t &amount);

		/// <summary>
		/// Return the time value as a number of seconds.
		/// </summary>
		/// <returns> Time in seconds. </returns>
		float AsSeconds() const;

		/// <summary>
		/// Return the time value as a number of milliseconds.
		/// </summary>
		/// <returns> Time in milliseconds. </returns>
		int32_t AsMilliseconds() const;

		/// <summary>
		/// Return the time value as a number of microseconds.
		/// </summary>
		/// <returns> Time in microseconds. </returns>
		int64_t AsMicroseconds() const;

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
	};
}
