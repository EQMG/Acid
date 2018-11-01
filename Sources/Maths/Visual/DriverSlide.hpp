#pragma once

#include "IDriver.hpp"

namespace acid
{
	/// <summary>
	/// A driver that slides to its destination using cosine interpolation.
	/// </summary>
	class ACID_EXPORT DriverSlide :
		public IDriver
	{
	private:
		float m_start;
		float m_end;
	public:
		/// <summary>
		/// Creates a new slide driver.
		/// </summary>
		/// <param name="start"> The start value. </param>
		/// <param name="end"> The end value. </param>
		/// <param name="length"> The time to get to the end value. </param>
		DriverSlide(const float &start, const float &end, const Time &length);

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
	protected:
		float Calculate(const float &factor) override;
	};
}
