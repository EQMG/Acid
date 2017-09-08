#pragma once

#include "../engine/Engine.hpp"
#include "../inputs/buttonmouse.hpp"
#include "../inputs/buttonjoystick.hpp"
#include "../inputs/axisjoystick.hpp"
#include "../uis/uiobject.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a virtual cursor that will be used to determine if a ui action was preformed by a device.
	/// </summary>
	class uiselector
	{
	private:
		float m_cursorX;
		float m_cursorY;

		bool m_leftClick;
		bool m_rightClick;
		bool m_leftWasClick;
		bool m_rightWasClick;

		buttonmouse *m_mouseLeft;
		buttonmouse *m_mouseRight;

		bool m_joysticksInitialized;
		int m_selectedJoystick;
		AxisJoystick *m_joystickAxisX;
		AxisJoystick *m_joystickAxisY;
		ButtonJoystick *m_joystickLeft;
		ButtonJoystick *m_joystickRight;
	public:
		uiselector();

		~uiselector();

		/// <summary>
		/// Sets up the joystick settings to be used for controlling the virtual cursor.
		/// </summary>
		/// <param name="joystick"> The joystick ID to attach though. </param>
		/// <param name="joystickLeftClick"> The joystick key to be used as the left click. </param>
		/// <param name="joystickRightClick"> The joystick key to be used as the right click. </param>
		/// <param name="joystickAxisX"> The joystick axis to be used for moving the x axis. </param>
		/// <param name="joystickAxisY"> The joystick axis to be used for moving the y axis. </param>
		void load(const int &joystick, const int &joystickLeftClick, const int &joystickRightClick, const int &joystickAxisX, const int &joystickAxisY);

		void update(const bool &paused);

		/// <summary>
		/// Gets if the object provided has the cursor hovered above it.
		/// </summary>
		/// <param name="object"> The object to check with.
		/// </param>
		/// <returns> If the object has the cursor inside of its box. </returns>
		bool isSelected(const UiObject &object);

		void cancelWasEvent();

		float getCursorX() { return m_cursorX; }

		float getCursorY() { return m_cursorY; }

		bool getLeftClick() { return m_leftClick; }

		bool getRightClick() { return m_rightClick; }

		bool wasLeftClick() { return m_leftWasClick; }

		bool wasRightClick() { return m_rightWasClick; }
	};
}
