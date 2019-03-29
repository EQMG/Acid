#pragma once

#include "IDriver.hpp"

namespace acid
{
/// <summary>
/// A driver that fades from start to end.
/// </summary>
template<typename T>
class DriverFade :
	public IDriver<T>
{
public:
	/// <summary>
	/// Creates a new fade driver.
	/// </summary>
	/// <param name="start"> The fade start interval (0.0-1.0). </param>
	/// <param name="end"> The fade end interval (0.0-1.0). </param>
	/// <param name="peak"> The peak value. </param>
	/// <param name="length"> The time taken to get to the end. </param>
	DriverFade(const T &start, const T &end, const T &peak, const Time &length) :
		IDriver<T>(length),
		m_start(start),
		m_end(end),
		m_peak(peak)
	{
	}

	/// <summary>
	/// Gets the start interval.
	/// </summary>
	/// <returns> The start interval. </returns>
	const T &GetStart() const { return m_start; }

	/// <summary>
	/// Sets the start interval (0.0-1.0).
	/// </summary>
	/// <param name="start"> The new start interval. </param>
	void SetStart(const T &start) { m_start = start; }

	/// <summary>
	/// Gets the end interval.
	/// </summary>
	/// <returns> The end interval. </returns>
	const T &GetEnd() const { return m_end; }

	/// <summary>
	/// Sets the end interval (0.0-1.0).
	/// </summary>
	/// <param name="end"> The new end interval. </param>
	void SetEnd(const T &end) { m_end = end; }

	/// <summary>
	/// Gets the peak value.
	/// </summary>
	/// <returns> The peak value. </returns>
	const T &GetPeak() const { return m_peak; }

	/// <summary>
	/// Sets the peak value.
	/// </summary>
	/// <param name="peak"> The new peak value. </param>
	void SetPeak(const T &peak) { m_peak = peak; }

protected:
	T Calculate(const float &factor) override
	{
		if (factor < m_start)
		{
			return factor / m_start * m_peak;
		}

		if (factor > m_end)
		{
			return (1.0f - (factor - m_end) / (1.0f - m_end)) * m_peak;
		}

		return m_peak;
	}

private:
	T m_start;
	T m_end;
	T m_peak;
};
}
