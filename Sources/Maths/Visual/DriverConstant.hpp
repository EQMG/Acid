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
	public:
		/// <summary>
		/// Creates a new constant driver.
		/// </summary>
		/// <param name="constant"> The constant value. </param>
		explicit DriverConstant(const float &constant);

		/// <summary>
		/// Gets the constant.
		/// </summary>
		/// <returns> The constant. </returns>
		const float &GetConstant() const { return m_constant; }

		/// <summary>
		/// Sets the constant.
		/// </summary>
		/// <param name="constant"> The new constant. </param>
		void SetConstant(const float &constant) { m_constant = constant; };
	protected:
		float Calculate(const float &factor) override;
	private:
		float m_constant;
	};
}
