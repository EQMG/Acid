#pragma once

#include <array>
#include "Display/Display.hpp"
#include "Engine/Engine.hpp"

namespace acid
{
	enum MouseButton
	{
		MOUSE_BUTTON_1 = 0,
		MOUSE_BUTTON_2 = 1,
		MOUSE_BUTTON_3 = 2,
		MOUSE_BUTTON_4 = 3,
		MOUSE_BUTTON_5 = 4,
		MOUSE_BUTTON_6 = 5,
		MOUSE_BUTTON_7 = 6,
		MOUSE_BUTTON_8 = 7,
		MOUSE_BUTTON_LEFT = 0,
		MOUSE_BUTTON_RIGHT = 1,
		MOUSE_BUTTON_MIDDLE = 2,
		MOUSE_BUTTON_BEGIN_RANGE = MOUSE_BUTTON_1,
		MOUSE_BUTTON_END_RANGE = MOUSE_BUTTON_8,
		MOUSE_BUTTON_RANGE_SIZE = (MOUSE_BUTTON_8 - MOUSE_BUTTON_1 + 1),
		MOUSE_BUTTON_MAX_ENUM = 0x7FFFFFFF
	};

	/// <summary>
	/// A module used for the creation, updating and destruction of the mouse.
	/// </summary>
	class ACID_EXPORT Mouse :
		public IModule
	{
	private:
		std::array<bool, MOUSE_BUTTON_END_RANGE> m_mouseButtons;
		std::string m_mousePath;

		float m_lastMousePositionX;
		float m_lastMousePositionY;
		float m_mousePositionX;
		float m_mousePositionY;
		float m_mouseDeltaX;
		float m_mouseDeltaY;
		float m_mouseDeltaWheel;
		bool m_displaySelected;

		bool m_lastCursorDisabled;
		bool m_cursorDisabled;

		friend void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset);

		friend void CallbackMouseButton(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);

		friend void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos);

		friend void CallbackCursorEnter(GLFWwindow *window, int32_t entered);

	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Mouse *Get() { return Engine::Get()->GetModule<Mouse>(); }

		Mouse();

		~Mouse();

		void Update() override;

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
		bool GetButton(const MouseButton &mouseButton) const;

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
