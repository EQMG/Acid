#pragma once

namespace flounder {
	/// <summary>
	/// Represents a driver that changes over time.
	/// </summary>
	class valuedriver
	{
	private:
		float m_length;
		float m_currentTime;
	public:
		/// <summary>
		/// Creates a new driver with a length.
		/// </summary>
		/// <param name="length"> The drivers length. </param>
		valuedriver(const float length);

		/// <summary>
		/// Deconstructor for value driver.
		/// </summary>
		~valuedriver();

		/// <summary>
		/// Updates the driver with the passed time.
		/// </summary>
		/// <param name="delta"> The time between the last update.
		/// </param>
		/// <returns> The calculated value. </returns>
		float update(float delta);
	protected:
		/// <summary>
		/// Calculates the new value.
		/// </summary>
		/// <param name="time"> The time into the drivers life.
		/// </param>
		/// <returns> The calculated value. </returns>
		virtual float calculateValue(float time) = 0;
	};
}
