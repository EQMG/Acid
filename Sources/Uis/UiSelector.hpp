#pragma once

#include <array>
#include <optional>
#include "Devices/Mouse.hpp"
#include "Inputs/AxisJoystick.hpp"
#include "Inputs/ButtonJoystick.hpp"
#include "UiObject.hpp"

namespace acid
{
	/// <summary>
	/// Represents a virtual cursor that uses a joystick as input.
	/// </summary>
	class ACID_EXPORT VirtualJoystick
	{
	public:
		explicit VirtualJoystick(const JoystickPort &port = JOYSTICK_1, const uint32_t &joystickAxisX = 0, const uint32_t &joystickAxisY = 1, const std::vector<uint32_t> &inputButtons = {0, 1}) :
			m_port(port),
			m_axisX(AxisJoystick(port, joystickAxisX)),
			m_axisY(AxisJoystick(port, joystickAxisY)),
			m_inputButtons(std::array<std::optional<ButtonJoystick>, MOUSE_BUTTON_END_RANGE>())
		{
			for (const auto &inputButton : inputButtons)
			{
				m_inputButtons[inputButton] = ButtonJoystick(m_port, inputButton);
			}
		}

		const JoystickPort &GetPort() const { return m_port; }

		void SetPort(const JoystickPort &port) { m_port = port; }

		const AxisJoystick &GetAxisX() const { return m_axisX; }

		void SetAxisX(const AxisJoystick &axisX) { m_axisX = axisX; }

		const AxisJoystick &GetAxisY() const { return m_axisY; }

		void SetAxisY(const AxisJoystick &axisY) { m_axisY = axisY; }

		const std::optional<ButtonJoystick> &GetInputButton(const MouseButton &button) const { return m_inputButtons[button]; }

		void SetInputButton(const MouseButton &button, const ButtonJoystick &inputButton) { m_inputButtons[button] = inputButton; }
	private:
		JoystickPort m_port;
		AxisJoystick m_axisX;
		AxisJoystick m_axisY;
		std::array<std::optional<ButtonJoystick>, MOUSE_BUTTON_END_RANGE> m_inputButtons;
	};

	/// <summary>
	/// Represents a virtual cursor that will be used to determine if a ui action was preformed by a device.
	/// </summary>
	class ACID_EXPORT UiSelector
	{
	public:
		UiSelector();

		void Update(const VirtualJoystick *virtualJoystick);

		/// <summary>
		/// Gets if the object provided has the cursor hovered above it.
		/// </summary>
		/// <param name="object"> The object to check with.
		/// </param>
		/// <returns> If the object has the cursor inside of its box. </returns>
		bool IsSelected(const UiObject &object) const;

		void CancelWasEvent(const MouseButton &button);

		const float &GetCursorX() const { return m_cursorX; }

		const float& GetCursorY() const { return m_cursorY; }

		const bool &IsDown(const MouseButton &button) const { return m_selectorMice[button].m_isDown; }

		const bool &WasDown(const MouseButton &button) const { return m_selectorMice[button].m_wasDown; }
	private:
		struct SelectorMouse
		{
			MouseButton m_mouseButton;
			bool m_isDown;
			bool m_wasDown;
		};

		float m_cursorX;
		float m_cursorY;
		std::array<SelectorMouse, MOUSE_BUTTON_END_RANGE> m_selectorMice;
	};
}
