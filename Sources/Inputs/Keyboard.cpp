#include "Keyboard.hpp"

namespace fl
{
	void CallbackKey(WsiShell shell, WsiKey key, int scancode, WsiAction action, WsiModifierFlags modFlags)
	{
		if (key < 0 || key > WSI_KEY_END_RANGE)
		{
			fprintf(stderr, "Invalid action attempted with key: '%i'\n", key);
		}
		else
		{
			Keyboard::Get()->m_keyboardKeys[key] = action;
		}
	}

	Keyboard::Keyboard() :
		IModule(),
		m_keyboardKeys(std::array<WsiAction, WSI_KEY_END_RANGE>()),
		m_keyboardChar(0)
	{
		// Sets the default state of the keys to released.
		for (unsigned int i = 0; i < WSI_KEY_END_RANGE; i++)
		{
			m_keyboardKeys[i] = WSI_ACTION_RELEASE;
		}

		// Sets the keyboards callbacks.
		WsiShellCallbacks *callbacks;
		wsiGetShellCallbacks(Display::Get()->GetWsiShell(), &callbacks);
		callbacks->pfnKey = CallbackKey;
	}

	Keyboard::~Keyboard()
	{
	}

	void Keyboard::Update()
	{
	}

	bool Keyboard::GetKey(const WsiKey &key) const
	{
		if (key < 0 || key > WSI_KEY_END_RANGE)
		{
			return false;
		}

		return m_keyboardKeys[key] != WSI_ACTION_RELEASE;
	}

	int Keyboard::GetChar() const
	{
		return m_keyboardChar;
	}
}
