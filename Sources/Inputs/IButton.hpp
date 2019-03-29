#pragma once

#include "Devices/Window.hpp"
#include "StdAfx.hpp"

namespace acid
{
/// <summary>
/// Interface for a binary input device.
/// </summary>
class ACID_EXPORT IButton
{
public:
	virtual ~IButton() = default;

	/// <summary>
	/// Returns whether this button is currently pressed.
	/// </summary>
	/// <returns> True if the button is pressed, false otherwise. </returns>
	virtual bool IsDown() const = 0;

	/// <summary>
	/// Gets if the key is down and was not down before. Key press recognized as one click.
	/// </summary>
	/// <returns> Is the key down and was not down before? </returns>
	bool WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}

	Delegate<void(InputAction, BitMask<InputMod>)> &GetOnButton() { return m_onButton; }

protected:
	Delegate<void(InputAction, BitMask<InputMod>)> m_onButton;
	bool m_wasDown;
};
}
