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
		float m_amplitude;
	public:
		/// <summary>
		/// Creates a new sine wave driver.
		/// </summary>
		/// <param name="min"> The min value. </param>
		/// <param name="max"> The max value. </param>
		/// <param name="length"> The length between two waves. </param>
		DriverSinwave(const float &min, const float &max, const float &length);

		/// <summary>
		/// Deconstructor for sin wave driver.
		/// </summary>
		~DriverSinwave();

	protected:
		float Calculate(const float &time) override;
	};
}
