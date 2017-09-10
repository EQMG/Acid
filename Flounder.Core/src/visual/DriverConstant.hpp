#pragma once

#include "IDriver.hpp"

namespace Flounder
{
	/// <summary>
	/// A driver that has a constant value.
	/// </summary>
	class DriverConstant :
		public IDriver
	{
	private:
		float m_value;
	public:
		/// <summary>
		/// Creates a new constant driver.
		/// </summary>
		/// <param name="constant"> The constant value. </param>
		DriverConstant(const float &constant);

		/// <summary>
		/// Deconstructor for constant driver.
		/// </summary>
		~DriverConstant();
	protected:
		float Calculate(const float &time) override;
	};
}
