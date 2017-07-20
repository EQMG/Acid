#pragma once

#include "idriver.h"

namespace flounder 
{
	/// <summary>
	/// A driver that fades from start to end.
	/// </summary>
	class driverfade : 
		public idriver
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
		driverfade(const float &start, const float &end, const float &peak, const float &length);

		/// <summary>
		/// Deconstructor for fade driver.
		/// </summary>
		~driverfade();

		/// <summary>
		/// Gets the start time.
		/// </summary>
		/// <returns> The start time. </returns>
		inline float &getStart() { return m_start; }

		/// <summary>
		/// Sets the start time.
		/// </summary>
		/// <param name="start"> The new start time. </param>
		inline void setStart(const float &start) { m_start = start; }

		/// <summary>
		/// Gets the end time.
		/// </summary>
		/// <returns> The ebd time. </returns>
		inline float &getEnd() { return m_end; }

		/// <summary>
		/// Sets the end time.
		/// </summary>
		/// <param name="end"> The new end time. </param>
		inline void setEnd(const float &end) { m_end = end; }

		/// <summary>
		/// Gets the peak value.
		/// </summary>
		/// <returns> The peak value. </returns>
		inline float &getPeak() { return m_peak; }

		/// <summary>
		/// Sets the peak value.
		/// </summary>
		/// <param name="peak"> The new peak value. </param>
		inline void setPeak(const float &peak) { m_peak = peak; }
	protected:
		float calculate(const float &time) override;
	};
}
