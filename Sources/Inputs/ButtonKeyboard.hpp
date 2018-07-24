#pragma once

#include <vector>
#include "Keyboard.hpp"
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
		std::vector<Key> m_keys;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new button keyboard.
		/// </summary>
		/// <param name="keys"> The keys on the keyboard being checked. </param>
		ButtonKeyboard(const std::vector<Key> &keys);

		/// <summary>
		/// Deconstructor for the button keyboard.
		/// </summary>
		~ButtonKeyboard();

		bool IsDown() const override;

		bool WasDown() override;

		std::vector<Key> GetKeys() const { return m_keys; }

		void SetKeys(const std::vector<Key> &keys) { m_keys = keys; }
	};
}
