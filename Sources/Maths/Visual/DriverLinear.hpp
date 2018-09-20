#pragma once

#include "IDriver.hpp"

namespace acid
{
	/// <summary>
	/// A driver that linearly increases its value.
	/// </summary>
	class ACID_EXPORT DriverLinear :
		public IDriver
	{
	private:
		float m_start;
		float m_end;
	public:
		/// <summary>
		/// Creates a new linear driver.
		/// </summary>
		/// <param name="startValue"> The start value. </param>
		/// <param name="endValue"> The end value. </param>
		/// <param name="length"> The time to go between values. </param>
		DriverLinear(const float &startValue, const float &endValue, const float &length);

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
		/// <returns> The ebd time. </returns>
		float GetEnd() const { return m_end; }

		/// <summary>
		/// Sets the end time.
		/// </summary>
		/// <param name="end"> The new end time. </param>
		void SetEnd(const float &end) { m_end = end; }
	protected:
		float Calculate(const float &time) override;
	};
}
