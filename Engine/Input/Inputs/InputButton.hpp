#pragma once

#include "Devices/Windows.hpp"
#include "Utils/StreamFactory.hpp"
#include "InputAxis.hpp"

namespace acid {
/**
 * @brief Interface for a binary input device.
 */
class ACID_INPUT_EXPORT InputButton : public StreamFactory<InputButton>, public virtual rocket::trackable {
public:
	virtual ~InputButton() = default;

	/**
	 * Returns whether this button is currently pressed.
	 * @return True if the button is pressed, false otherwise.
	 */
	virtual bool IsDown() const { return false; }

	/**
	 * Gets if the key is down and was not down before. Key press recognized as one click.
	 * @return Is the key down and was not down before?
	 */
	bool WasDown() {
		auto stillDown = wasDown && IsDown();
		wasDown = IsDown();
		return wasDown == !stillDown;
	}

	virtual InputAxis::ArgumentDescription GetArgumentDescription() const { return {}; }

	/**
	 * Called when the button changes state.
	 * @return The delegate.
	 */
	rocket::signal<void(InputAction, bitmask::bitmask<InputMod>)> &OnButton() { return onButton; }

	bool IsInverted() const { return inverted; }
	void SetInverted(bool inverted) { this->inverted = inverted; }

protected:
	rocket::signal<void(InputAction, bitmask::bitmask<InputMod>)> onButton;
	bool inverted = false;

private:
	bool wasDown = false;
};
}
