#pragma once

#include "IDriver.hpp"

namespace Flounder
{
	/// <summary>
	/// A driver that linearly increases its value.
	/// </summary>
	class DriverLinear :
		public IDriver
	{
	private:
		float m_startValue;
		float m_difference;
	public:
		/// <summary>
		/// Creates a new linear driver.
		/// </summary>
		/// <param name="startValue"> The start value. </param>
		/// <param name="endValue"> The end value. </param>
		/// <param name="length"> The time to go between values. </param>
		DriverLinear(const float &startValue, const float &endValue, const float &length);

		/// <summary>
		/// Deconstructor for linear driver.
		/// </summary>
		~DriverLinear();
	protected:
		float Calculate(const float &time) override;
	};
}
