#pragma once

#include "idriver.h"

namespace flounder {
	/// <summary>
	/// A driver that uses a sine wave.
	/// </summary>
	class driversinwave : public idriver
	{
	private:
		float m_min;
		float m_amplitude;
	public:
		/// <summary>
		/// Creates a new sine wave driver.
		/// </summary>
		/// <param name="min"> The min value. </param>
		/// <param name="max"> The max value. </param>
		/// <param name="length"> The length between two waves. </param>
		driversinwave(const float min, const float max, const float length);

		/// <summary>
		/// Deconstructor for sin wave driver.
		/// </summary>
		~driversinwave();

	protected:
		float calculate(const float time) override;
	};
}
