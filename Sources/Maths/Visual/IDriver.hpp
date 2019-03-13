#pragma once

#include "Maths/Time.hpp"

namespace acid
{
/// <summary>
/// Represents
/// a
/// driver
/// that
/// changes
/// over
/// time.
/// </summary>
template<typename T>
class IDriver
{
  public:
	/// <summary>
	/// Creates
	/// a
	/// new
	/// driver
	/// with
	/// a
	/// length.
	/// </summary>
	/// <param
	/// name="length">
	/// The
	/// drivers
	/// length.
	/// </param>
	explicit IDriver(const Time& length) : m_length(length), m_actualTime(Time::Zero), m_currentTime(Time::Zero) {}

	virtual ~IDriver() = default;

	/// <summary>
	/// Updates
	/// the
	/// driver
	/// with
	/// the
	/// passed
	/// time.
	/// </summary>
	/// <param
	/// name="delta">
	/// The
	/// time
	/// between
	/// the
	/// last
	/// update.
	/// </param>
	/// <returns>
	/// The
	/// calculated
	/// value.
	/// </returns>
	T Update(const Time& delta)
	{
		m_actualTime += delta;
		m_currentTime += delta;
		m_currentTime = m_currentTime % m_length;
		auto time = m_currentTime / m_length;
		return Calculate(time);
	}

	/// <summary>
	/// Gets
	/// the
	/// length.
	/// </summary>
	/// <returns>
	/// The
	/// length.
	/// </returns>
	const Time& GetLength() const
	{
		return m_length;
	}

	/// <summary>
	/// Sets
	/// the
	/// length.
	/// </summary>
	/// <param
	/// name="length">
	/// The
	/// new
	/// length.
	/// </param>
	void SetLength(const Time& length)
	{
		m_length = length;
	}

  protected:
	/// <summary>
	/// Calculates
	/// the
	/// new
	/// value.
	/// </summary>
	/// <param
	/// name="time">
	/// The
	/// time
	/// into
	/// the
	/// drivers
	/// life.
	/// </param>
	/// <returns>
	/// The
	/// calculated
	/// value.
	/// </returns>
	virtual T Calculate(const float& factor) = 0;

	Time m_length;
	Time m_actualTime;
	Time m_currentTime;
};
}
