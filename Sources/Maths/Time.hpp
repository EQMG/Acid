#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
/// <summary>
/// Represents a time value stored in microseconds.
/// </summary>
class ACID_EXPORT Time
{
public:
	/// <summary>
	/// Constructor for time. This function is internal. To construct time values, use Time::Seconds, Time::Milliseconds or Time::Microseconds instead.
	/// </summary>
	/// <param name="microseconds"> Number of microseconds. </param>
	explicit Time(const int64_t &microseconds = 0) noexcept;

	/// <summary>
	/// Constructs a time value from a number of seconds.
	/// </summary>
	/// <param name="amount"> Number of seconds. </param>
	/// <returns> Time value constructed from the amount of seconds. </returns>
	template<typename T = float> static Time Seconds(const T &amount)
	{
		return Time(static_cast<int64_t>(amount * static_cast<T>(1000000)));
	}

	/// <summary>
	/// Constructs a time value from a number of milliseconds.
	/// </summary>
	/// <param name="amount"> Number of milliseconds. </param>
	/// <returns> Time value constructed from the amount of milliseconds. </returns>
	template<typename T = int32_t> static Time Milliseconds(const T &amount)
	{
		return Time(static_cast<int64_t>(amount * static_cast<T>(1000)));
	}

	/// <summary>
	/// Constructs a time value from a number of microseconds.
	/// </summary>
	/// <param name="amount"> Number of microseconds. </param>
	/// <returns> Time value constructed from the amount of microseconds. </returns>
	template<typename T = int64_t> static Time Microseconds(const T &amount)
	{
		return Time(static_cast<int64_t>(amount));
	}

	/// <summary>
	/// Return the time value as a number of seconds.
	/// </summary>
	/// <returns> Time in seconds. </returns>
	template<typename T = float> auto AsSeconds() const
	{
		return static_cast<T>(m_microseconds) / static_cast<T>(1000000);
	}

	/// <summary>
	/// Return the time value as a number of milliseconds.
	/// </summary>
	/// <returns> Time in milliseconds. </returns>
	template<typename T = int32_t> auto AsMilliseconds() const
	{
		return static_cast<T>(m_microseconds) / static_cast<T>(1000);
	}

	/// <summary>
	/// Return the time value as a number of microseconds.
	/// </summary>
	/// <returns> Time in microseconds. </returns>
	template<typename T = int64_t> auto AsMicroseconds() const
	{
		return static_cast<T>(m_microseconds);
	}

	void Decode(const Metadata &metadata);

	void Encode(Metadata &metadata) const;

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

	static const Time Zero;
	static const Time Min;
	static const Time Max;
private:
	int64_t m_microseconds;
};
}
