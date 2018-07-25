#include "Keyboard.hpp"

namespace acid
{
	void CallbackChar(const char *str)
	{
		Keyboard::Get()->m_char = str[0];
	}

	void CallbackKey(Key key, bool isDown)
	{
		if (key < 0 || key > KEY_END_RANGE)
		{
			fprintf(stderr, "Invalid action attempted with key: '%i'\n", key);
		}
		else
		{
			Keyboard::Get()->m_keyboardKeys[key] = isDown;
		}
	}

	Keyboard::Keyboard() :
		IModule(),
		m_keyboardKeys(std::array<bool, KEY_END_RANGE>()),
		m_char(0)
	{
		auto shell = Display::Get()->GetShell();

		// Sets the default state of the keys to released.
		for (unsigned int i = 0; i < KEY_END_RANGE; i++)
		{
			m_keyboardKeys[i] = false;
		}

		// Sets the keyboards callbacks.
		shell->SetCallbackChar(CallbackChar);
		shell->SetCallbackKey(CallbackKey);
	}

	Keyboard::~Keyboard()
	{
	}

	void Keyboard::Update()
	{
	}

	bool Keyboard::GetKey(const Key &key) const
	{
		if (key < 0 || key > KEY_END_RANGE)
		{
			return false;
		}

		return m_keyboardKeys[key];
	}

	char Keyboard::GetChar() const
	{
		return m_char;
	}
}
