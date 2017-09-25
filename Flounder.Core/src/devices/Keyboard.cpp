#include "Keyboard.hpp"

namespace Flounder
{
	void CallbackKey(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		// TODO: Play with mods.

		if (key < 0 || key > GLFW_KEY_LAST)
		{
			printf("Invalid action attempted with key: '%i'.\n", key);
		}
		else
		{
			Keyboard::Get()->m_keyboardKeys[key] = action;
		}
	}

	void CallbackChar(GLFWwindow *window, unsigned int codepoint)
	{
		Keyboard::Get()->m_keyboardChar = codepoint;
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
		glfwSetKeyCallback(Display::Get()->GetWindow(), CallbackKey);
		glfwSetCharCallback(Display::Get()->GetWindow(), CallbackChar);
	}

	Keyboard::~Keyboard()
	{
		delete m_keyboardKeys;
	}

	void Keyboard::Update()
	{
	}

	bool Keyboard::GetKey(const int &key) const
	{
		if (key < 0 || key > GLFW_KEY_LAST + 1)
		{
			return false;
		}

		return m_keyboardKeys[key] != GLFW_RELEASE;
	}

	int Keyboard::GetChar() const
	{
		return m_keyboardChar;
	}
}
