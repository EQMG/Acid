#pragma once

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
public:
	/// <summary>
	/// Creates a new button keyboard.
	/// </summary>
	/// <param name="keys"> The key on the keyboard being checked. </param>
	explicit ButtonKeyboard(const Key &key);

	bool IsDown() const override;

	const Key &GetKey() const { return m_key; }

	void SetKey(const Key &key) { m_key = key; }

private:
	Key m_key;
};
}
