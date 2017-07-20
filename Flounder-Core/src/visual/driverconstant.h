#pragma once

#include "idriver.h"

namespace flounder
{
	/// <summary>
	/// A driver that has a constant value.
	/// </summary>
	class driverconstant : 
		public idriver
	{
	private:
		float m_value;
	public:
		/// <summary>
		/// Creates a new constant driver.
		/// </summary>
		/// <param name="constant"> The constant value. </param>
		driverconstant(const float &constant);

		/// <summary>
		/// Deconstructor for constant driver.
		/// </summary>
		~driverconstant();
	protected:
		float calculate(const float &time) override;
	};
}
