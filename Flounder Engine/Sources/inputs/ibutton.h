#pragma once

namespace flounder
{
	/// <summary>
	/// Interface for a binary input device.
	/// </summary>
	class ibutton
	{
		/// <summary>
		/// Returns whether this button is currently pressed.
		/// </summary>
		/// <returns> True if the button is pressed, false otherwise. </returns>
	public:
		virtual bool isDown() = 0;

		/// <summary>
		/// Gets if the key is down and was not down before. Key press recognized as one click.
		/// </summary>
		/// <returns> Is the key down and was not down before? </returns>
		virtual bool wasDown() = 0;
	};
}
