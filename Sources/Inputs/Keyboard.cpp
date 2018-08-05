#include "Keyboard.hpp"

#include <GLFW/glfw3.h>

namespace acid
{
	void CallbackKey(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key < 0 || key > Key::KEY_END_RANGE)
		{
			fprintf(stderr, "Invalid action attempted with key: '%i'\n", key);
		}
		else
		{
			Keyboard::Get()->m_keyboardKeys[key] = action != GLFW_RELEASE;
		}
	}

	void CallbackChar(GLFWwindow *window, uint32_t codepoint)
	{
		Keyboard::Get()->m_char = static_cast<char>(codepoint);
	}

	Keyboard::Keyboard() :
		IModule(),
		m_keyboardKeys(std::array<bool, KEY_END_RANGE>()),
		m_char(0)
	{
		// Sets the default state of the keys to released.
		for (uint32_t i = 0; i < KEY_END_RANGE; i++)
		{
			m_keyboardKeys[i] = false;
		}

		// Sets the keyboards callbacks.
		glfwSetKeyCallback(Display::Get()->GetGlfwWindow(), CallbackKey);
		glfwSetCharCallback(Display::Get()->GetGlfwWindow(), CallbackChar);
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
