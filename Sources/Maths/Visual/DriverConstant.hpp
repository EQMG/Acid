#pragma once

#include "IDriver.hpp"

namespace acid
{
	/// <summary>
	/// A driver that has a constant value.
	/// </summary>
	class ACID_EXPORT DriverConstant :
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

		~DriverConstant();

	protected:
		float Calculate(const float &time) override;
	};
}
