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
		float m_start;
		float m_end;
		float m_peak;
	public:
		/// <summary>
		/// Creates a new fade driver.
		/// </summary>
		/// <param name="start"> The fade start interval (0.0-1.0). </param>
		/// <param name="end"> The fade end interval (0.0-1.0). </param>
		/// <param name="peak"> The peak value. </param>
		/// <param name="length"> The time taken to get to the end. </param>
		DriverFade(const float &start, const float &end, const float &peak, const Time &length);

		/// <summary>
		/// Gets the start interval.
		/// </summary>
		/// <returns> The start interval. </returns>
		float GetStart() const { return m_start; }

		/// <summary>
		/// Sets the start interval (0.0-1.0).
		/// </summary>
		/// <param name="start"> The new start interval. </param>
		void SetStart(const float &start) { m_start = start; }

		/// <summary>
		/// Gets the end interval.
		/// </summary>
		/// <returns> The end interval. </returns>
		float GetEnd() const { return m_end; }

		/// <summary>
		/// Sets the end interval (0.0-1.0).
		/// </summary>
		/// <param name="end"> The new end interval. </param>
		void SetEnd(const float &end) { m_end = end; }

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
