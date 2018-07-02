#pragma once

#include "Engine/Engine.hpp"
#include "Display/Display.hpp"

namespace fl
{
	/// <summary>
	/// A module used for the creation, updating and destruction of the keyboard keys.
	/// </summary>
	class FL_EXPORT Keyboard :
		public IModule
	{
	private:
		std::array<WsiAction, WSI_KEY_END_RANGE> m_keyboardKeys;
		int m_keyboardChar;

		friend void CallbackKey(WsiShell shell, WsiKey key, WsiAction action, uint32_t modsCount, WsiModifier *mods);

	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Keyboard *Get()
		{
			return Engine::Get()->GetModule<Keyboard>();
		}

		/// <summary>
		/// Creates a new keyboard module.
		/// </summary>
		Keyboard();

		/// <summary>
		/// Deconstructor for the keyboard module.
		/// </summary>
		~Keyboard() override;

		void Update() override;

		/// <summary>
		/// Gets whether or not a particular key is currently pressed.
		/// <p>GLFW Actions: GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT</p>
		/// </summary>
		/// <param name="key"> The key to test. </param>
		/// <returns> If the key is currently pressed. </returns>
		bool GetKey(const WsiKey &key) const;

		/// <summary>
		/// Gets the current user input, ASCII Dec value.
		/// </summary>
		/// <returns> The current keyboard char. </returns>
		int GetChar() const;
	};
}
