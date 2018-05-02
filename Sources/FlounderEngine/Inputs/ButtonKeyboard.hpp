#pragma once

#include <vector>
#include "IButton.hpp"

namespace fl
{
	/// <summary>
	/// Keys from a keyboard.
	/// </summary>
	class FL_EXPORT ButtonKeyboard :
		public IButton
	{
	private:
		std::vector<int> m_keys;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new button keyboard.
		/// </summary>
		/// <param name="keys"> The keys on the keyboard being checked. </param>
		ButtonKeyboard(const std::vector<int> &keys);

		/// <summary>
		/// Deconstructor for the button keyboard.
		/// </summary>
		~ButtonKeyboard();

		bool IsDown() const override;

		bool WasDown() override;
	};
}
