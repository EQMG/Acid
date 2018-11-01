#pragma once

#include "Maths/Maths.hpp"
#include "Maths/Time.hpp"

namespace acid
{
	/// <summary>
	/// Represents a driver that changes over time.
	/// </summary>
	class ACID_EXPORT IDriver
	{
	private:
		Time m_length;
		Time m_actualTime;
		Time m_currentTime;
	public:
		/// <summary>
		/// Creates a new driver with a length.
		/// </summary>
		/// <param name="length"> The drivers length. </param>
		explicit IDriver(const Time &length) :
			m_length(length),
			m_actualTime(Time::ZERO),
			m_currentTime(Time::ZERO)
		{
		}

		/// <summary>
		/// Updates the driver with the passed time.
		/// </summary>
		/// <param name="delta"> The time between the last update. </param>
		/// <returns> The calculated value. </returns>
		float Update(const Time &delta)
		{
			m_actualTime += delta;
			m_currentTime += delta;
			m_currentTime = m_currentTime % m_length;
			float time = m_currentTime / m_length;
			return Calculate(time);
		}

		/// <summary>
		/// Gets the length.
		/// </summary>
		/// <returns> The length. </returns>
		Time GetLength() const { return m_length; }

		/// <summary>
		/// Sets the length.
		/// </summary>
		/// <param name="length"> The new length. </param>
		void SetLength(const Time &length) { m_length = length; }
	protected:
		/// <summary>
		/// Calculates the new value.
		/// </summary>
		/// <param name="time"> The time into the drivers life.
		/// </param>
		/// <returns> The calculated value. </returns>
		virtual float Calculate(const float &factor) = 0;

		Time GetActualTime() const { return m_actualTime; }
	};
}
