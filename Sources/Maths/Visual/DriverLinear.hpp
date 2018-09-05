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

		~DriverLinear();

	protected:
		float Calculate(const float &time) override;
	};
}
