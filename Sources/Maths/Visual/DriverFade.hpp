#pragma once

#include "IDriver.hpp"

namespace acid
{
	/// <summary>
	/// A driver that fades from start to end.
	/// </summary>
	class ACID_EXPORT DriverFade :
		public IDriver
	{
	private:
		Time m_start;
		Time m_end;
		float m_peak;
	public:
		/// <summary>
		/// Creates a new fade driver.
		/// </summary>
		/// <param name="start"> The start time. </param>
		/// <param name="end"> The end time. </param>
		/// <param name="peak"> The peak value. </param>
		/// <param name="length"> The time taken to get to the end. </param>
		DriverFade(const Time &start, const Time &end, const float &peak, const Time &length);

		/// <summary>
		/// Gets the start time.
		/// </summary>
		/// <returns> The start time. </returns>
		Time GetStart() const { return m_start; }

		/// <summary>
		/// Sets the start time.
		/// </summary>
		/// <param name="start"> The new start time. </param>
		void SetStart(const Time &start) { m_start = start; }

		/// <summary>
		/// Gets the end time.
		/// </summary>
		/// <returns> The end time. </returns>
		Time GetEnd() const { return m_end; }

		/// <summary>
		/// Sets the end time.
		/// </summary>
		/// <param name="end"> The new end time. </param>
		void SetEnd(const Time &end) { m_end = end; }

		/// <summary>
		/// Gets the peak value.
		/// </summary>
		/// <returns> The peak value. </returns>
		float GetPeak() const { return m_peak; }

		/// <summary>
		/// Sets the peak value.
		/// </summary>
		/// <param name="peak"> The new peak value. </param>
		void SetPeak(const float &peak) { m_peak = peak; }
	protected:
		float Calculate(const float &factor) override;
	};
}
