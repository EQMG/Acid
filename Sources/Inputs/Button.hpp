#pragma once

#include "Devices/Window.hpp"

namespace acid
{
/**
 * @brief Interface for a binary input device.
 */
class ACID_EXPORT Button :
	public Observer
{
public:
	virtual ~Button() = default;

	/**
	 * Returns whether this button is currently pressed.
	 * @return True if the button is pressed, false otherwise.
	 */
	virtual bool IsDown() const = 0;

	/**
	 * Gets if the key is down and was not down before. Key press recognized as one click.
	 * @return Is the key down and was not down before?
	 */
	bool WasDown()
	{
		auto stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}

	/**
	 * Called when the button changes state.
	 * @return The delegate.
	 */
	Delegate<void(InputAction, BitMask<InputMod>)> &OnButton() { return m_onButton; }

protected:
	Delegate<void(InputAction, BitMask<InputMod>)> m_onButton;

private:
	bool m_wasDown{};
};
}
