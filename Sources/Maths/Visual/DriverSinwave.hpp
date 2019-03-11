#pragma once

#include "Maths/Maths.hpp"
#include "IDriver.hpp"

namespace acid
{
	/// <summary>
	/// A driver that uses a sine wave.
	/// </summary>
	template<typename T>
	class DriverSinwave :
		public IDriver<T>
	{
	public:
		/// <summary>
		/// Creates a new sine wave driver.
		/// </summary>
		/// <param name="min"> The min value. </param>
		/// <param name="max"> The max value. </param>
		/// <param name="length"> The length between two waves. </param>
		DriverSinwave(const T &min, const T &max, const Time &length) :
			IDriver<T>(length),
			m_min(min),
			m_max(max)
		{
		}

		/// <summary>
		/// Gets the min value.
		/// </summary>
		/// <returns> The min value. </returns>
		const T &GetMin() const { return m_min; }

		/// <summary>
		/// Sets the min value.
		/// </summary>
		/// <param name="min"> The new min value. </param>
		void SetMin(const T &min) { m_min = min; }

		/// <summary>
		/// Gets the max value.
		/// </summary>
		/// <returns> The max value. </returns>
		const T &GetMax() const { return m_max; }

		/// <summary>
		/// Sets the max value.
		/// </summary>
		/// <param name="max"> The new max value. </param>
		void SetMax(const T &max) { m_max = max; }
	protected:
		T Calculate(const float &factor) override
		{
			float value = 0.5f + std::sin(2.0f * Maths::Pi * factor) * 0.5f;
			return m_min + value * (m_max - m_min);
		}

	private:
		T m_min;
		T m_max;
	};
}
