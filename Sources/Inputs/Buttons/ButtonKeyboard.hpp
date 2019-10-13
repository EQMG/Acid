#pragma once

#include "Devices/Keyboard.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Button input from the keyboard input device.
 */
class ACID_EXPORT ButtonKeyboard : public Button::Registrar<ButtonKeyboard> {
public:
	/**
	 * Creates a new button keyboard.
	 * @param key The key on the keyboard being checked.
	 */
	explicit ButtonKeyboard(Key key = Key::Unknown);

	bool IsDown() const override;

	Axis::ArgumentDescription GetArgumentDescription() const override;

	Key GetKey() const { return m_key; }
	void SetKey(Key key) { m_key = key; }

	friend const Node &operator>>(const Node &node, ButtonKeyboard &buttonKeyboard);
	friend Node &operator<<(Node &node, const ButtonKeyboard &buttonKeyboard);

private:
	static bool registered;

	Key m_key;
};
}
