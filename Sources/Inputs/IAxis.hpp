#pragma once

#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// Interface for an axis based input device.
	/// </summary>
	class ACID_EXPORT IAxis
	{
	public:
		/// <summary>
		/// Gets the current value along the axis. -1 is smallest input, 1 is largest input.
		/// </summary>
		/// <returns> The current value of the axis in the range (-1, 1). </returns>
		virtual float GetAmount() const = 0;
	};
}
