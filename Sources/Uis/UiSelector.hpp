#pragma once

#include <array>
#include <optional>
#include "Inputs/AxisJoystick.hpp"
#include "Inputs/ButtonJoystick.hpp"
#include "Inputs/Mouse.hpp"
#include "UiObject.hpp"

namespace acid
{
	class ACID_EXPORT SelectorJoystick
	{
	private:
		JoystickPort m_joystick;
		AxisJoystick m_axisX;
		AxisJoystick m_axisY;
		std::array<std::optional<ButtonJoystick>, MOUSE_BUTTON_END_RANGE> m_inputButtons;
	public:
		SelectorJoystick(const JoystickPort &joystick = JOYSTICK_1, const uint32_t &joystickAxisX = 0, const uint32_t &joystickAxisY = 1, const std::vector<uint32_t> &inputButtons = {0, 1}) :
			m_joystick(joystick),
			m_axisX(AxisJoystick(joystick, {joystickAxisX})),
			m_axisY(AxisJoystick(joystick, {joystickAxisY})),
			m_inputButtons(std::array<std::optional<ButtonJoystick>, MOUSE_BUTTON_END_RANGE>())
		{
			for (auto &inputButton : inputButtons)
			{
				m_inputButtons[inputButton] = ButtonJoystick(joystick, {inputButton});
			}
		}

		~SelectorJoystick()
		{
		}

		JoystickPort GetJoystick() const { return m_joystick; }

		void SetJoystick(const JoystickPort &joystick) { m_joystick = joystick; }

		AxisJoystick GetAxisX() const { return m_axisX; }

		void SetAxisX(const AxisJoystick &axisX) { m_axisX = axisX; }

		AxisJoystick GetAxisY() const { return m_axisY; }

		void SetAxisY(const AxisJoystick &axisY) { m_axisY = axisY; }

		std::optional<ButtonJoystick> GetInputButton(const MouseButton &button) const { return m_inputButtons[button]; }

		void SetInputButton(const MouseButton &button, const ButtonJoystick &inputButton) { m_inputButtons[button] = inputButton; }
	};

	struct UiSelectorMouse
	{
		MouseButton m_mouseButton;
		bool m_isDown;
		bool m_wasDown;
	};

	/// <summary>
	/// Represents a virtual cursor that will be used to determine if a ui action was preformed by a device.
	/// </summary>
	class ACID_EXPORT UiSelector
	{
	private:
		float m_cursorX;
		float m_cursorY;

		std::array<UiSelectorMouse, MOUSE_BUTTON_END_RANGE> m_selectorMice;
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

		bool IsDown(const MouseButton &button) const { return m_selectorMice[button].m_isDown; }

		bool WasDown(const MouseButton &button) const { return m_selectorMice[button].m_wasDown; }
	};
}
