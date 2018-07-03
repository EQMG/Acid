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
		char m_char;

		friend void CallbackChar(WsiShell shell, const char *str);

		friend void CallbackKey(WsiShell shell, WsiKey key, WsiAction action, WsiModifierFlags modFlags);

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
		/// <p>Actions: WSI_ACTION_PRESS, WSI_ACTION_RELEASE, WSI_ACTION_REPEAT</p>
		/// </summary>
		/// <param name="key"> The key to test. </param>
		/// <returns> If the key is currently pressed. </returns>
		bool GetKey(const WsiKey &key) const;

		/// <summary>
		/// Gets the current user input, ASCII Dec value.
		/// </summary>
		/// <returns> The current keyboard char. </returns>
		char GetChar() const;
	};
}
