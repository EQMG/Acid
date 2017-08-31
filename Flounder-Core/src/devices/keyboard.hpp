#pragma once

#include "../engine/Engine.hpp"

#include "display.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for the creation, updating and destruction of the keyboard keys.
	/// </summary>
	class keyboard : public IModule
	{
	private:
		int *m_keyboardKeys;
		int m_keyboardChar;

		friend void callbackKey(GLFWwindow *window, int key, int scancode, int action, int mods);
		friend void callbackChar(GLFWwindow *window, unsigned int codepoint);
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline keyboard *get()
		{
			return static_cast<keyboard*>(Engine::Get()->GetModule("keyboard"));
		}

		/// <summary>
		/// Creates a new keyboard module.
		/// </summary>
		keyboard();

		/// <summary>
		/// Deconstructor for the keyboard module.
		/// </summary>
		~keyboard() override;

		void Update() override;

		/// <summary>
		/// Gets whether or not a particular key is currently pressed.
		/// <p>GLFW Actions: GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT</p>
		/// </summary>
		/// <param name="key"> The key to test. </param>
		/// <returns> If the key is currently pressed. </returns>
		bool getKey(const int &key) const;

		/// <summary>
		/// Gets the current user input, ASCII Dec value.
		/// </summary>
		/// <returns> The current keyboard char. </returns>
		int getKeyboardChar() const;
	};
}
