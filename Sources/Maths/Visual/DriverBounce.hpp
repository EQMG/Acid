#pragma once

#include "IDriver.hpp"

namespace acid
{
	/// <summary>
	/// A bounce driver that uses a sine wave.
	/// </summary>
	class ACID_EXPORT DriverBounce :
		public IDriver
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
		DriverBounce(const float &start, const float &end, const float &length);

		~DriverBounce();

	protected:
		float Calculate(const float &time) override;
	};
}
