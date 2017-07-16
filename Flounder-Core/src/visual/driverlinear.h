#pragma once

#include "idriver.h"

namespace flounder {
	/// <summary>
	/// A driver that linearly increases its value.
	/// </summary>
	class driverlinear : public idriver
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
		driverlinear(const float &startValue, const float &endValue, const float &length);

		/// <summary>
		/// Deconstructor for linear driver.
		/// </summary>
		~driverlinear();
	protected:
		float calculate(const float &time) override;
	};
}
