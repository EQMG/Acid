#pragma once

#include "Devices/Joysticks.hpp"
#include "Devices/Windows.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiButtonInput.hpp"

namespace acid {
class ACID_UI_EXPORT UiGrabberInput : public UiObject {
public:
	UiGrabberInput();

	void UpdateObject() override;

	const std::string &GetTitle() const { return textTitle.GetString(); }
	void SetTitle(const std::string &string) { textTitle.SetString(string); }

protected:
	virtual std::string GetTextString() const = 0;

	void SetUpdating(bool updating);
	void UpdateValue();

	Gui background;
	Text textTitle;
	Text textValue;

	int32_t lastKey = 0;

	bool updating = false;
	bool mouseOver = false;
};

class ACID_UI_EXPORT UiGrabberJoystick : public UiGrabberInput {
public:
	UiGrabberJoystick();

	JoystickPort GetPort() const { return port; }
	void SetPort(JoystickPort port) { this->port = port; }

	uint32_t GetValue() const { return value; }
	void SetValue(uint32_t value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	rocket::signal<void(JoystickPort, uint32_t)> &OnValue() { return onValue; }

protected:
	std::string GetTextString() const override {
		return String::To(value);
	}

private:
	JoystickPort port = JoystickPort::_1; // -1
	uint32_t value = 0;
	rocket::signal<void(JoystickPort, uint32_t)> onValue;
};

class ACID_UI_EXPORT UiGrabberKeyboard : public UiGrabberInput {
public:
	UiGrabberKeyboard();

	Key GetValue() const { return value; }
	void SetValue(Key value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	rocket::signal<void(Key)> &OnValue() { return onValue; }

protected:
	std::string GetTextString() const override {
		return Window::ToString(value);
	}

private:
	Key value = Key::Unknown;
	rocket::signal<void(Key)> onValue;
};

class ACID_UI_EXPORT UiGrabberMouse : public UiGrabberInput {
public:
	UiGrabberMouse();

	MouseButton GetValue() const { return value; }
	void SetValue(MouseButton value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	rocket::signal<void(MouseButton)> &OnValue() { return onValue; }

protected:
	std::string GetTextString() const override {
		return String::To(static_cast<int32_t>(value));
	}

private:
	MouseButton value = MouseButton::_8;
	rocket::signal<void(MouseButton)> onValue;
};
}
