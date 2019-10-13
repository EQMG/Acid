#pragma once

#include "Helpers/Factory.hpp"
#include "Devices/Window.hpp"

namespace acid {
/**
 * @brief Interface for a binary input device.
 */
class ACID_EXPORT Button : public Factory<Button>, public virtual Observer {
public:
	virtual ~Button() = default;

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
		auto stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}

	/**
	 * Called when the button changes state.
	 * @return The delegate.
	 */
	Delegate<void(InputAction, BitMask<InputMod>)> &OnButton() { return m_onButton; }

	bool IsInverted() const { return m_inverted; }
	void SetInverted(bool inverted) { m_inverted = inverted; }

protected:
	Delegate<void(InputAction, BitMask<InputMod>)> m_onButton;
	bool m_inverted = false;

private:
	bool m_wasDown = false;
};
}
