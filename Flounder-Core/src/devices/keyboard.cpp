#include "Keyboard.hpp"

namespace Flounder
{
	void callbackKey(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		// TODO: Play with mods.

		if (key < 0 || key > GLFW_KEY_LAST)
		{
			printf("Invalid action attempted with key: '%i'.\n", key);
		}
		else
		{
			Keyboard::get()->m_keyboardKeys[key] = action;
		}
	}

	void callbackChar(GLFWwindow *window, unsigned int codepoint)
	{
		Keyboard::get()->m_keyboardChar = codepoint;
	}

	Keyboard::Keyboard() :
		IModule(),
		m_keyboardKeys(new int[GLFW_KEY_LAST + 1]),
		m_keyboardChar(0)
	{
		// Sets the default state of the keys to released.
		for (unsigned int i = 0; i < GLFW_KEY_LAST + 1; i++)
		{
			m_keyboardKeys[i] = GLFW_RELEASE;
		}

		// Sets the keyboards callbacks.
		glfwSetKeyCallback(Display::get()->getWindow(), callbackKey);
		glfwSetCharCallback(Display::get()->getWindow(), callbackChar);
	}

	Keyboard::~Keyboard()
	{
		delete m_keyboardKeys;
	}

	void Keyboard::Update()
	{
	}

	bool Keyboard::getKey(const int &key) const
	{
		if (key < 0 || key > GLFW_KEY_LAST + 1)
		{
			return false;
		}

		return m_keyboardKeys[key] != GLFW_RELEASE;
	}

	int Keyboard::getKeyboardChar() const
	{
		return m_keyboardChar;
	}
}
