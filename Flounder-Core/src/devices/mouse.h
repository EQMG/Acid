#pragma once

#include "../framework/framework.h"
#include "../maths/maths.h"

#include "display.h"

namespace flounder {
	/// <summary>
	/// A module used for the creation, updating and destruction of the mouse.
	/// </summary>
	class mouse : 
		public imodule
	{
	private:
		std::string m_customMouse;

		int *m_mouseButtons;
		double m_lastMousePositionX;
		double m_lastMousePositionY;
		double m_mousePositionX;
		double m_mousePositionY;
		double m_mouseDeltaX;
		double m_mouseDeltaY;
		double m_mouseDeltaWheel;
		bool m_displaySelected;

		bool m_cursorDisabled;
		bool m_lastCursorDisabled;

		friend void callbackScroll(GLFWwindow* window, double xoffset, double yoffset);
		friend void callbackMouseButton(GLFWwindow* window, int button, int action, int mods);
		friend void callbackCursorPos(GLFWwindow* window, double xpos, double ypos);
		friend void callbackCursorEnter(GLFWwindow* window, int entered);
	public:
		static mouse* get() 
		{
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

		void load(const std::string &customMouse);

		void init();

		void update();

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
		bool getButton(const unsigned int &button);

		/// <summary>
		/// Gets the mouses screen x position.
		/// </summary>
		/// <returns> The mouses x position. </returns>
		double &getPositionX();

		/// <summary>
		/// Gets the mouses screen y position.
		/// </summary>
		/// <returns> The mouses y position. </returns>
		double &getPositionY();

		/// <summary>
		/// Sets the mouse position by calling glfwSetCursorPos.
		/// </summary>
		/// <param name="cursorX"> The x position in screenspace. </param>
		/// <param name="cursorY"> The y position in screenspace. </param>
		void setPosition(const double &cursorX, const double &cursorY);
		
		/// <summary>
		/// Gets the mouses delta x.
		/// </summary>
		/// <returns> The mouses delta x. </returns>
		double &getDeltaX();

		/// <summary>
		/// Gets the mouses delta y.
		/// </summary>
		/// <returns> The mouses delta y. </returns>
		double &getDeltaY();

		/// <summary>
		/// Gets the mouses wheel delta.
		/// </summary>
		/// <returns> The mouses wheel delta. </returns>
		double &getDeltaWheel();
		
		/// <summary>
		/// Gets if the display is selected.
		/// </summary>
		/// <returns> If the display is selected. </returns>
		bool &isDisplaySelected();
		
		/// <summary>
		/// If the cursor is hidden, the mouse is the display locked if true.
		/// </summary>
		/// <returns> If the cursor is hidden. </returns>
		bool &isCursorDisabled();
	};
}
