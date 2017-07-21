#pragma once

#include "idriver.h"

namespace flounder
{
	/// <summary>
	/// A driver that slides to its destination using cosine interpolation.
	/// </summary>
	class driverslide :
		public idriver
	{
	private:
		float m_start;
		float m_end;
		float m_max;
		bool m_reachedTarget;
	public:
		/// <summary>
		/// Creates a new slide driver.
		/// </summary>
		/// <param name="start"> The start value. </param>
		/// <param name="end"> The end value. </param>
		/// <param name="length"> The time to get to the end value. </param>
		driverslide(const float &start, const float &end, const float &length);

		/// <summary>
		/// Deconstructor for slide driver.
		/// </summary>
		~driverslide();
	protected:
		float calculate(const float &time) override;
	};
}
