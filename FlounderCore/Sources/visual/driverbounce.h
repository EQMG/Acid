#pragma once

#include "idriver.h"

namespace flounder {
	/// <summary>
	/// A bounce driver that uses a sine wave.
	/// </summary>
	class driverbounce : public idriver
	{
	private:
		float m_start;
		float m_amplitude;
		float m_length;
	public:
		/// <summary>
		/// Creates a new sine wave driver.
		/// </summary>
		/// <param name="start"> The start value. </param>
		/// <param name="end"> The end value. </param>
		/// <param name="length"> The length between two waves. </param>
		driverbounce(const float &start, const float &end, const float &length);

		/// <summary>
		/// Deconstructor for bounce driver.
		/// </summary>
		~driverbounce();

	protected:
		float calculate(const float &time) override;
	};
}
