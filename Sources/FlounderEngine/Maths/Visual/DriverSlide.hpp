#pragma once

#include "IDriver.hpp"

namespace fl
{
	/// <summary>
	/// A driver that slides to its destination using cosine interpolation.
	/// </summary>
	class FL_EXPORT DriverSlide :
		public IDriver
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
		DriverSlide(const float &start, const float &end, const float &length);

		/// <summary>
		/// Deconstructor for slide driver.
		/// </summary>
		~DriverSlide();

	protected:
		float Calculate(const float &time) override;
	};
}
