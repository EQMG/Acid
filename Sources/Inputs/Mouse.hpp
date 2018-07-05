#pragma once

#include "Engine/Engine.hpp"
#include "Display/Display.hpp"

namespace fl
{
	/// <summary>
	/// A module used for the creation, updating and destruction of the mouse.
	/// </summary>
	class FL_EXPORT Mouse :
		public IModule
	{
	private:
		std::string m_mousePath;

		std::array<WsiAction, WSI_MOUSE_BUTTON_END_RANGE> m_mouseButtons;
		float m_mousePositionX;
		float m_mousePositionY;
		float m_mouseDeltaX;
		float m_mouseDeltaY;
		float m_mouseDeltaWheel;
		bool m_displaySelected;

		bool m_cursorDisabled;

		friend void CallbackCursorPosition(WsiShell shell, float x, float y, float dx, float dy);

		friend void CallbackCursorEnter(WsiShell shell, VkBool32 entered);

		friend void CallbackScroll(WsiShell shell, float x, float y);

		friend void CallbackMouseButton(WsiShell shell, WsiMouseButton mouseButton, WsiAction action);

	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Mouse *Get()
		{
			return Engine::Get()->GetModule<Mouse>();
		}

		/// <summary>
		/// Creates a new mouse module.
		/// </summary>
		Mouse();

		/// <summary>
		/// Deconstructor for the mouse module.
		/// </summary>
		~Mouse();

		void Update() override;

		/// <summary>
		/// Gets the mouses custom mouse file.
		/// </summary>
		/// <returns> The custom mouse file. </returns>
		std::string GetCustomMouse() const { return m_mousePath; }

		/// <summary>
		/// Sets the custom mouse file.
		/// </summary>
		/// <param name="filename"> The new custom mouse file. </param>
		void SetCustomMouse(const std::string &filename);

		/// <summary>
		/// Sets if the operating systems cursor is hidden whilst in the display.
		/// </summary>
		/// <param name="disabled"> If the system cursor should be disabled or hidden when not shown. </param>
		void SetCursorHidden(const bool &disabled);

		/// <summary>
		/// Gets whether or not a particular mouse button is currently pressed.
		/// <p>Actions: WSI_ACTION_PRESS, WSI_ACTION_RELEASE, WSI_ACTION_REPEAT</p>
		/// </summary>
		/// <param name="mouseButton"> The mouse button to test. </param>
		/// <returns> If the mouse button is currently pressed. </returns>
		bool GetButton(const WsiMouseButton &mouseButton) const;

		/// <summary>
		/// Gets the mouses screen x position.
		/// </summary>
		/// <returns> The mouses x position. </returns>
		float GetPositionX() const { return m_mousePositionX; }

		/// <summary>
		/// Gets the mouses screen y position.
		/// </summary>
		/// <returns> The mouses y position. </returns>
		float GetPositionY() const { return m_mousePositionY; }

		/// <summary>
		/// Sets the mouse position.
		/// </summary>
		/// <param name="cursorX"> The x position in screenspace. </param>
		/// <param name="cursorY"> The y position in screenspace. </param>
		void SetPosition(const float &cursorX, const float &cursorY);

		/// <summary>
		/// Gets the mouses delta x.
		/// </summary>
		/// <returns> The mouses delta x. </returns>
		float GetDeltaX() const { return m_mouseDeltaX; }

		/// <summary>
		/// Gets the mouses delta y.
		/// </summary>
		/// <returns> The mouses delta y. </returns>
		float GetDeltaY() const { return m_mouseDeltaY; }

		/// <summary>
		/// Gets the mouses wheel delta.
		/// </summary>
		/// <returns> The mouses wheel delta. </returns>
		float GetDeltaWheel() const { return m_mouseDeltaWheel; }

		/// <summary>
		/// Gets if the display is selected.
		/// </summary>
		/// <returns> If the display is selected. </returns>
		bool IsDisplaySelected() const { return m_displaySelected; }

		/// <summary>
		/// If the cursor is hidden, the mouse is the display locked if true.
		/// </summary>
		/// <returns> If the cursor is hidden. </returns>
		bool IsCursorDisabled() const { return m_cursorDisabled; }
	};
}
