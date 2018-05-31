#pragma once

#include "Inputs/AxisJoystick.hpp"
#include "Inputs/ButtonMouse.hpp"
#include "Inputs/ButtonJoystick.hpp"
#include "Uis/UiObject.hpp"

namespace fl
{
	struct FL_EXPORT SelectorJoystick
	{
	private:
		JoystickPort m_joystick;
		ButtonJoystick *m_clickLeft;
		ButtonJoystick *m_clickRight;
		AxisJoystick *m_axisX;
		AxisJoystick *m_axisY;
	public:
		SelectorJoystick(const JoystickPort &joystick = JOYSTICK_LAST, const int &joystickLeftClick = 0, const int &joystickRightClick = 1, const int &joystickAxisX = 0, const int &joystickAxisY = 1) :
			m_joystick(joystick),
			m_clickLeft(new ButtonJoystick(joystick, {joystickLeftClick})),
			m_clickRight(new ButtonJoystick(joystick, {joystickRightClick})),
			m_axisX(new AxisJoystick(joystick, {joystickAxisX})),
			m_axisY(new AxisJoystick(joystick, {joystickAxisY}))
		{
		}

		~SelectorJoystick()
		{
			delete m_clickLeft;
			delete m_clickRight;
			delete m_axisX;
			delete m_axisY;
		}

		JoystickPort GetJoystick() const { return m_joystick; }

		void SetJoystick(const JoystickPort &joystick) { m_joystick = joystick; }

		ButtonJoystick *GetClickLeft() const { return m_clickLeft; }

	//	void SetClickLeft(const ButtonJoystick &clickLeft) { m_clickLeft = clickLeft; }

		ButtonJoystick *GetClickRight() const { return m_clickRight; }

	//	void SetClickRight(const ButtonJoystick &clickRight) { m_clickRight = clickRight; }

		AxisJoystick *GetAxisX() const { return m_axisX; }

	//	void SetAxisX(const AxisJoystick &axisX) { m_axisX = axisX; }

		AxisJoystick *GetAxisY() const { return m_axisY; }

	//	void SetAxisY(const AxisJoystick &axisY) { m_axisY = axisY; }
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

		ButtonMouse *m_mouseLeft;
		ButtonMouse *m_mouseRight;
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
