#pragma once

#include "Inputs/AxisJoystick.hpp"
#include "Inputs/ButtonMouse.hpp"
#include "Inputs/ButtonJoystick.hpp"
#include "Uis/UiObject.hpp"

namespace fl
{
	class FL_EXPORT SelectorJoystick
	{
	private:
		WsiJoystick m_joystick;
		ButtonJoystick m_clickLeft;
		ButtonJoystick m_clickRight;
		AxisJoystick m_axisX;
		AxisJoystick m_axisY;
	public:
		SelectorJoystick(const WsiJoystick &joystick = WSI_JOYSTICK_END_RANGE, const uint32_t &joystickLeftClick = 0, const uint32_t &joystickRightClick = 1, const uint32_t &joystickAxisX = 0, const uint32_t &joystickAxisY = 1) :
			m_joystick(joystick),
			m_clickLeft(ButtonJoystick(joystick, {joystickLeftClick})),
			m_clickRight(ButtonJoystick(joystick, {joystickRightClick})),
			m_axisX(AxisJoystick(joystick, {joystickAxisX})),
			m_axisY(AxisJoystick(joystick, {joystickAxisY}))
		{
		}

		~SelectorJoystick()
		{
		}

		WsiJoystick GetJoystick() const { return m_joystick; }

		void SetJoystick(const WsiJoystick &joystick) { m_joystick = joystick; }

		ButtonJoystick GetClickLeft() const { return m_clickLeft; }

		void SetClickLeft(const ButtonJoystick &clickLeft) { m_clickLeft = clickLeft; }

		ButtonJoystick GetClickRight() const { return m_clickRight; }

		void SetClickRight(const ButtonJoystick &clickRight) { m_clickRight = clickRight; }

		AxisJoystick GetAxisX() const { return m_axisX; }

		void SetAxisX(const AxisJoystick &axisX) { m_axisX = axisX; }

		AxisJoystick GetAxisY() const { return m_axisY; }

		void SetAxisY(const AxisJoystick &axisY) { m_axisY = axisY; }
	};

	/// <summary>
	/// Represents a virtual cursor that will be used to determine if a ui action was preformed by a device.
	/// </summary>
	class FL_EXPORT UiSelector
	{
	private:
		float m_cursorX;
		float m_cursorY;

		bool m_leftClick;
		bool m_rightClick;
		bool m_leftWasClick;
		bool m_rightWasClick;

		ButtonMouse m_mouseLeft;
		ButtonMouse m_mouseRight;
	public:
		UiSelector();

		~UiSelector();

		void Update(const bool &paused, const SelectorJoystick &selectorJoystick);

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
