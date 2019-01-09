#pragma once

#include <vector>
#include "Devices/Keyboard.hpp"
#include "IButton.hpp"

namespace acid
{
	/// <summary>
	/// Keys from a keyboard.
	/// </summary>
	class ACID_EXPORT ButtonKeyboard :
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
		explicit ButtonKeyboard(const std::vector<Key> &keys);

		bool IsDown() const override;

		bool WasDown() override;

		const std::vector<Key> &GetKeys() const { return m_keys; }

		void SetKeys(const std::vector<Key> &keys) { m_keys = keys; }
	};
}
