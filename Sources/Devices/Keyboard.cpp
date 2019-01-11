#include "Keyboard.hpp"

#include <GLFW/glfw3.h>

namespace acid
{
	void CallbackKey(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
	{
		if (key < 0 || key > Key::KEY_END_RANGE)
		{
			return;
		}

		Keyboard::Get()->m_keyboardKeys[key] = static_cast<InputAction>(action);
	}

	void CallbackChar(GLFWwindow *window, uint32_t codepoint)
	{
		Keyboard::Get()->m_char = static_cast<char>(codepoint);
	}

	Keyboard::Keyboard() :
		m_keyboardKeys(std::array<InputAction, KEY_END_RANGE>()),
		m_char(0)
	{
		// Sets the default state of the keys to released.
		for (uint32_t i = 0; i < KEY_END_RANGE; i++)
		{
			m_keyboardKeys[i] = INPUT_ACTION_RELEASE;
		}

		// Sets the keyboards callbacks.
		glfwSetKeyCallback(Window::Get()->GetWindow(), CallbackKey);
		glfwSetCharCallback(Window::Get()->GetWindow(), CallbackChar);
	}

	void Keyboard::Update()
	{
	}

	InputAction Keyboard::GetKey(const Key &key) const
	{
		if (key < 0 || key >= KEY_END_RANGE)
		{
			return INPUT_ACTION_RELEASE;
		}

		return m_keyboardKeys[key];
	}
}
