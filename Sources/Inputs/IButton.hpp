#pragma once

#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// Interface for a binary input device.
	/// </summary>
	class ACID_EXPORT IButton
	{
	public:
		/// <summary>
		/// Returns whether this button is currently pressed.
		/// </summary>
		/// <returns> True if the button is pressed, false otherwise. </returns>
		virtual bool IsDown() const = 0;

		/// <summary>
		/// Gets if the key is down and was not down before. Key press recognized as one click.
		/// </summary>
		/// <returns> Is the key down and was not down before? </returns>
		virtual bool WasDown() = 0;
	};
}
