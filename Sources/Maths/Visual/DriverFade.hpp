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
		/// <param name="start"> The start time. </param>
		/// <param name="end"> The end time. </param>
		/// <param name="peak"> The peak value. </param>
		/// <param name="length"> The time taken to get to the end. </param>
		DriverFade(const float &start, const float &end, const float &peak, const float &length);

		/// <summary>
		/// Gets the start time.
		/// </summary>
		/// <returns> The start time. </returns>
		float GetStart() const { return m_start; }

		/// <summary>
		/// Sets the start time.
		/// </summary>
		/// <param name="start"> The new start time. </param>
		void SetStart(const float &start) { m_start = start; }

		/// <summary>
		/// Gets the end time.
		/// </summary>
		/// <returns> The end time. </returns>
		float GetEnd() const { return m_end; }

		/// <summary>
		/// Sets the end time.
		/// </summary>
		/// <param name="end"> The new end time. </param>
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
		float Calculate(const float &time) override;
	};
}
