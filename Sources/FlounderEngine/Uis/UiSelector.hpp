﻿#pragma once

#include "Inputs/Axes/AxisJoystick.hpp"
#include "Inputs/Buttons/ButtonMouse.hpp"
#include "Inputs/Buttons/ButtonJoystick.hpp"
#include "Uis/UiObject.hpp"

namespace Flounder
{
	struct F_HIDDEN SelectorJoystick
	{
	public:
		unsigned int joystick;
		ButtonJoystick *clickLeft;
		ButtonJoystick *clickRight;
		AxisJoystick *axisX;
		AxisJoystick *axisY;
	};

	/// <summary>
	/// Represents a virtual cursor that will be used to determine if a ui action was preformed by a device.
	/// </summary>
	class F_EXPORT UiSelector
	{
	private:
		float m_cursorX;
		float m_cursorY;

		bool m_leftClick;
		bool m_rightClick;
		bool m_leftWasClick;
		bool m_rightWasClick;

		ButtonMouse *m_mouseLeft;
		ButtonMouse *m_mouseRight;

		SelectorJoystick *m_selectorJoystick;
	public:
		UiSelector();

		~UiSelector();

		/// <summary>
		/// Sets up the joystick settings to be used for controlling the virtual cursor.
		/// </summary>
		/// <param name="joystick"> The joystick ID to attach though. </param>
		/// <param name="joystickLeftClick"> The joystick key to be used as the left click. </param>
		/// <param name="joystickRightClick"> The joystick key to be used as the right click. </param>
		/// <param name="joystickAxisX"> The joystick axis to be used for moving the x axis. </param>
		/// <param name="joystickAxisY"> The joystick axis to be used for moving the y axis. </param>
		void Load(const unsigned int &joystick, const int &joystickLeftClick, const int &joystickRightClick, const int &joystickAxisX, const int &joystickAxisY);

		void Update(const bool &paused);

		/// <summary>
		/// Gets if the object provided has the cursor hovered above it.
		/// </summary>
		/// <param name="object"> The object to check with.
		/// </param>
		/// <returns> If the object has the cursor inside of its box. </returns>
		bool IsSelected(const UiObject &object) const;

		void CancelWasEvent();

		float GetCursorX() const { return m_cursorX; }

		float GetCursorY() const { return m_cursorY; }

		bool GetLeftClick() const { return m_leftClick; }

		bool GetRightClick() const { return m_rightClick; }

		bool WasLeftClick() const { return m_leftWasClick; }

		bool WasRightClick() const { return m_rightWasClick; }
	};
}
