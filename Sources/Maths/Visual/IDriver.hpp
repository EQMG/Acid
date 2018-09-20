#pragma once

#include "Maths/Maths.hpp"

namespace acid
{
	/// <summary>
	/// Represents a driver that changes over time.
	/// </summary>
	class ACID_EXPORT IDriver
	{
	private:
		float m_length;
		float m_actualTime;
		float m_currentTime;
	public:
		/// <summary>
		/// Creates a new driver with a length.
		/// </summary>
		/// <param name="length"> The drivers length. </param>
		explicit IDriver(const float &length) :
			m_length(length),
			m_actualTime(0.0f),
			m_currentTime(0.0f)
		{
		}

		/// <summary>
		/// Updates the driver with the passed time.
		/// </summary>
		/// <param name="delta"> The time between the last update. </param>
		/// <returns> The calculated value. </returns>
		float Update(const float &delta)
		{
			m_actualTime += delta;
			m_currentTime += delta;
			m_currentTime = std::fmod(m_currentTime, m_length);
			float time = m_currentTime / m_length;
			return Calculate(time);
		}

		/// <summary>
		/// Gets the length.
		/// </summary>
		/// <returns> The length. </returns>
		float GetLength() const { return m_length; }

		/// <summary>
		/// Sets the length.
		/// </summary>
		/// <param name="length"> The new length. </param>
		void SetLength(const float &length) { m_length = length; }
	protected:
		/// <summary>
		/// Calculates the new value.
		/// </summary>
		/// <param name="time"> The time into the drivers life.
		/// </param>
		/// <returns> The calculated value. </returns>
		virtual float Calculate(const float &time) = 0;

		float GetActualTime() const { return m_actualTime; }
	};
}
