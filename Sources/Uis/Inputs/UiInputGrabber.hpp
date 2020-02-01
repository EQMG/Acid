#pragma once

#include "Devices/Joysticks.hpp"
#include "Devices/Keyboard.hpp"
#include "Devices/Mouse.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiInputButton.hpp"

namespace acid {
class ACID_EXPORT UiInputGrabber : public UiObject {
public:
	UiInputGrabber();

	void UpdateObject() override;

	const std::string &GetTitle() const { return m_textTitle.GetString(); }
	void SetTitle(const std::string &string) { m_textTitle.SetString(string); }

protected:
	virtual std::string GetTextString() const = 0;

	void SetUpdating(bool updating);
	void UpdateValue();

	Gui m_background;
	Text m_textTitle;
	Text m_textValue;

	int32_t m_lastKey = 0;

	bool m_updating = false;
	bool m_mouseOver = false;
};

class ACID_EXPORT UiGrabberJoystick : public UiInputGrabber {
public:
	UiGrabberJoystick();

	uint32_t GetPort() const { return m_port; }
	void SetPort(uint32_t port) { m_port = port; }

	uint32_t GetValue() const { return m_value; }
	void SetValue(uint32_t value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t, uint32_t)> &OnValue() { return m_onValue; }

protected:
	std::string GetTextString() const override {
		return String::To(m_value);
	}

private:
	uint32_t m_port = 0; // -1
	uint32_t m_value = 0;
	Delegate<void(uint32_t, uint32_t)> m_onValue;
};

class ACID_EXPORT UiGrabberKeyboard : public UiInputGrabber {
public:
	UiGrabberKeyboard();

	Key GetValue() const { return m_value; }
	void SetValue(Key value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(Key)> &OnValue() { return m_onValue; }

protected:
	std::string GetTextString() const override {
		return Keyboard::ToString(m_value);
	}

private:
	Key m_value = Key::Unknown;
	Delegate<void(Key)> m_onValue;
};

class ACID_EXPORT UiGrabberMouse : public UiInputGrabber {
public:
	UiGrabberMouse();

	MouseButton GetValue() const { return m_value; }
	void SetValue(MouseButton value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(MouseButton)> &OnValue() { return m_onValue; }

protected:
	std::string GetTextString() const override {
		return String::To(static_cast<int32_t>(m_value));
	}

private:
	MouseButton m_value = MouseButton::Last;
	Delegate<void(MouseButton)> m_onValue;
};
}
