#pragma once

#include "IDriver.hpp"

namespace acid
{
	/// <summary>
	/// A driver that uses a sine wave.
	/// </summary>
	class ACID_EXPORT DriverSinwave :
		public IDriver
	{
	private:
		float m_min;
		float m_max;
	public:
		/// <summary>
		/// Creates a new sine wave driver.
		/// </summary>
		/// <param name="min"> The min value. </param>
		/// <param name="max"> The max value. </param>
		/// <param name="length"> The length between two waves. </param>
		DriverSinwave(const float &min, const float &max, const float &length);

		/// <summary>
		/// Gets the min value.
		/// </summary>
		/// <returns> The min value. </returns>
		float GetMin() const { return m_min; }

		/// <summary>
		/// Sets the min value.
		/// </summary>
		/// <param name="min"> The new min value. </param>
		void SetMin(const float &min) { m_min = min; }

		/// <summary>
		/// Gets the max value.
		/// </summary>
		/// <returns> The max value. </returns>
		float GetMax() const { return m_max; }

		/// <summary>
		/// Sets the max value.
		/// </summary>
		/// <param name="max"> The new max value. </param>
		void SetMax(const float &max) { m_max = max; }
	protected:
		float Calculate(const float &time) override;
	};
}
