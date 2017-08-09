#pragma once

#include "../framework/framework.h"
#include "../maths/maths.h"

#include "display.h"

namespace flounder
{
	/// <summary>
	/// A module used for the creation, updating and destruction of the mouse.
	/// </summary>
	class mouse :
		public imodule
	{
	private:
		std::string m_customMouse;

		int *m_mouseButtons;
		float m_lastMousePositionX;
		float m_lastMousePositionY;
		float m_mousePositionX;
		float m_mousePositionY;
		float m_mouseDeltaX;
		float m_mouseDeltaY;
		float m_mouseDeltaWheel;
		bool m_displaySelected;

		bool m_cursorDisabled;
		bool m_lastCursorDisabled;

		friend void callbackScroll(GLFWwindow *window, double xoffset, double yoffset);
		friend void callbackMouseButton(GLFWwindow *window, int button, int action, int mods);
		friend void callbackCursorPos(GLFWwindow *window, double xpos, double ypos);
		friend void callbackCursorEnter(GLFWwindow *window, int entered);
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static mouse *get()
		{
			return static_cast<mouse*>(framework::get()->getInstance("mouse"));
		}

		/// <summary>
		/// Creates a new mouse module.
		/// </summary>
		mouse();

		/// <summary>
		/// Deconstructor for the mouse module.
		/// </summary>
		~mouse();

		void update() override;

		/// <summary>
		/// Gets the mouses custom mouse file.
		/// </summary>
		/// <returns> The custom mouse file. </returns>
		std::string getCustomMouse() const { return m_customMouse; }

		/// <summary>
		/// Sets the custom mouse file.
		/// </summary>
		/// <param name="title"> The new custom mouse file. </param>
		void setCustomMouse(const std::string &customMouse);

		/// <summary>
		/// Sets if the operating systems cursor is hidden whilst in the display.
		/// </summary>
		/// <param name="disabled"> If the system cursor should be disabled or hidden when not shown. </param>
		void setCursorHidden(const bool &disabled);

		/// <summary>
		/// Gets whether or not a particular mouse button is currently pressed.
		/// <p>GLFW Actions: GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT</p>
		/// </summary>
		/// <param name="button"> The mouse button to test. </param>
		/// <returns> If the mouse button is currently pressed. </returns>
		inline bool getButton(const int &button) const { return m_mouseButtons[button] != GLFW_RELEASE; }

		/// <summary>
		/// Gets the mouses screen x position.
		/// </summary>
		/// <returns> The mouses x position. </returns>
		inline float getPositionX() const { return m_mousePositionX; }

		/// <summary>
		/// Gets the mouses screen y position.
		/// </summary>
		/// <returns> The mouses y position. </returns>
		inline float getPositionY() const { return m_mousePositionY; }

		/// <summary>
		/// Sets the mouse position by calling glfwSetCursorPos.
		/// </summary>
		/// <param name="cursorX"> The x position in screenspace. </param>
		/// <param name="cursorY"> The y position in screenspace. </param>
		void setPosition(const float &cursorX, const float &cursorY);

		/// <summary>
		/// Gets the mouses delta x.
		/// </summary>
		/// <returns> The mouses delta x. </returns>
		inline float getDeltaX() const { return m_mouseDeltaX; }

		/// <summary>
		/// Gets the mouses delta y.
		/// </summary>
		/// <returns> The mouses delta y. </returns>
		inline float getDeltaY() const { return m_mouseDeltaY; }

		/// <summary>
		/// Gets the mouses wheel delta.
		/// </summary>
		/// <returns> The mouses wheel delta. </returns>
		inline float getDeltaWheel() const { return m_mouseDeltaWheel; }

		/// <summary>
		/// Gets if the display is selected.
		/// </summary>
		/// <returns> If the display is selected. </returns>
		inline bool isDisplaySelected() const { return m_displaySelected; }

		/// <summary>
		/// If the cursor is hidden, the mouse is the display locked if true.
		/// </summary>
		/// <returns> If the cursor is hidden. </returns>
		inline bool isCursorDisabled() const { return m_cursorDisabled; }
	};
}
