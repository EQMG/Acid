#pragma once

#include "../framework/framework.h"
#include "../logger/logger.h"

#include "display.h"

namespace flounder {
	/// <summary>
	/// A module used for the creation, updating and destruction of the mouse.
	/// </summary>
	class mouse : public module
	{
	private:
	//	file *customMouse;

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

		friend static void callbackScroll(GLFWwindow* window, double xoffset, double yoffset);
		friend static void callbackMouseButton(GLFWwindow* window, int button, int action, int mods);
		friend static void callbackCursorPos(GLFWwindow* window, double xpos, double ypos);
		friend static void callbackCursorEnter(GLFWwindow* window, int entered);
	public:
		static mouse* get() {
			return (mouse*) framework::get()->getInstance("mouse");
		}

		/// <summary>
		/// Creates a new GLFW mouse manager.
		/// </summary>
		mouse();

		/// <summary>
		/// Deconstructor for the mouse.
		/// </summary>
		~mouse();

		void update();

		/// <summary>
		/// Sets if the operating systems cursor is hidden whilst in the display.
		/// </summary>
		/// <param name="disabled"> If the system cursor should be disabled or hidden when not shown. </param>
		void setCursorHidden(bool disabled);

		/// <summary>
		/// Gets whether or not a particular mouse button is currently pressed.
		/// <p>GLFW Actions: GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT</p>
		/// </summary>
		/// <param name="button"> The mouse button to test.
		/// </param>
		/// <returns> If the mouse button is currently pressed. </returns>
		bool getButton(int button);

		/// <summary>
		/// Gets the mouses screen x position.
		/// </summary>
		/// <returns> The mouses x position. </returns>
		float getPositionX();

		/// <summary>
		/// Gets the mouses screen y position.
		/// </summary>
		/// <returns> The mouses y position. </returns>
		float getPositionY();

		void setPosition(float cursorX, float cursorY);
		
		/// <summary>
		/// Gets the mouses delta x.
		/// </summary>
		/// <returns> The mouses delta x. </returns>
		float getDeltaX();

		/// <summary>
		/// Gets the mouses delta y.
		/// </summary>
		/// <returns> The mouses delta y. </returns>
		float getDeltaY();

		/// <summary>
		/// Gets the mouses wheel delta.
		/// </summary>
		/// <returns> The mouses wheel delta. </returns>
		float getDeltaWheel();
		
		/// <summary>
		/// Gets if the display is selected.
		/// </summary>
		/// <returns> If the display is selected. </returns>
		bool isDisplaySelected();
		
		/// <summary>
		/// If the cursor is hidden, the mouse is the display locked if true.
		/// </summary>
		/// <returns> If the cursor is hidden. </returns>
		bool isCursorDisabled();
	};
}
