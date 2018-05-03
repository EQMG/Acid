#pragma once

#include "Engine/Exports.hpp"

namespace fl
{
	/// <summary>
	/// Interface for an axis based input device.
	/// </summary>
	class FL_EXPORT IAxis
	{
	public:
		/// <summary>
		/// Creates a new axis.
		/// </summary>
		IAxis()
		{
		}

		/// <summary>
		/// Deconstructor for the axis.
		/// </summary>
		virtual ~IAxis()
		{
		}

		/// <summary>
		/// Gets the current value along the axis. -1 is smallest input, 1 is largest input.
		/// </summary>
		/// <returns> The current value of the axis in the range (-1, 1). </returns>
		virtual float GetAmount() const = 0;
	};
}
