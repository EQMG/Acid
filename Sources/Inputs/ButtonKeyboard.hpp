#pragma once

#include "Devices/Keyboard.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Button input from the keyboard input device.
 */
class ACID_EXPORT
ButtonKeyboard
:
public
Button
{
public:
	/**
	 * Creates a new button keyboard.
	 * @param key The key on the keyboard being checked.
	 */
	explicit ButtonKeyboard(const Key &key);

	bool IsDown() const override;

	const Key &GetKey() const { return m_key; }

	void SetKey(const Key &key) { m_key = key; }

private:
	Key m_key;
};
}
