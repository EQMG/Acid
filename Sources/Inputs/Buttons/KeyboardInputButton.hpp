#pragma once

#include "Devices/Keyboard.hpp"
#include "Inputs/InputButton.hpp"

namespace acid {
/**
 * @brief InputButton input from the keyboard input device.
 */
class ACID_EXPORT KeyboardInputButton : public InputButton::Registrar<KeyboardInputButton> {
	static const bool Registered;
public:
	/**
	 * Creates a new button keyboard.
	 * @param key The key on the keyboard being checked.
	 */
	explicit KeyboardInputButton(Key key = Key::Unknown);

	bool IsDown() const override;

	InputAxis::ArgumentDescription GetArgumentDescription() const override;

	Key GetKey() const { return key; }
	void SetKey(Key key) { this->key = key; }

	friend const Node &operator>>(const Node &node, KeyboardInputButton &inputButton);
	friend Node &operator<<(Node &node, const KeyboardInputButton &inputButton);

private:
	Key key;
};
}
