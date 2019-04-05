#pragma once

#include "Devices/Joysticks.hpp"
#include "Devices/Keyboard.hpp"
#include "Devices/Mouse.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputGrabber :
	public UiObject
{
public:
	UiInputGrabber(UiObject *parent, const std::string &title, const UiBound &rectangle);

	void UpdateObject() override;
	
	const std::string &GetTitle() const { return m_textTitle.GetString(); }

	void SetTitle(const std::string &title) { m_textTitle.SetString(title); }

protected:
	void SetUpdating(const bool &updating);

	void UpdateValue();

	virtual std::string GetTextString() const = 0;

	Gui m_background;
	Text m_textTitle;
	Text m_textValue;

	int32_t m_lastKey;

	bool m_updating;
	bool m_mouseOver;
};

class ACID_EXPORT UiGrabberJoystick :
	public UiInputGrabber
{
public:
	UiGrabberJoystick(UiObject *parent, const std::string &title, const uint32_t &port, const uint32_t &value,
		const UiBound &rectangle = UiBound(Vector2f(0.0f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Size));

	const uint32_t &GetPort() const { return m_port; }

	void SetPort(const uint32_t &port) { m_port = port; }

	const uint32_t &GetValue() const { return m_value; }

	void SetValue(const uint32_t &value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t, uint32_t)> &OnValue() { return m_onValue; }

protected:
	std::string GetTextString() const override
	{
		return String::To(m_value);
	}

private:
	uint32_t m_port;
	uint32_t m_value;
	Delegate<void(uint32_t, uint32_t)> m_onValue;
};

class ACID_EXPORT UiGrabberKeyboard :
	public UiInputGrabber
{
public:
	UiGrabberKeyboard(UiObject *parent, const std::string &title, const Key &value,
		const UiBound &rectangle = UiBound(Vector2f(0.0f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Size));

	const Key &GetValue() const { return m_value; }

	void SetValue(const Key &value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(Key)> &OnValue() { return m_onValue; }

protected:
	std::string GetTextString() const override
	{
		return Keyboard::ToString(m_value);
	}

private:
	Key m_value;
	Delegate<void(Key)> m_onValue;
};

class ACID_EXPORT UiGrabberMouse :
	public UiInputGrabber
{
public:
	UiGrabberMouse(UiObject *parent, const std::string &title, const MouseButton &value,
		const UiBound &rectangle = UiBound(Vector2f(0.0f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Size));

	const MouseButton &GetValue() const { return m_value; }

	void SetValue(const MouseButton &value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(MouseButton)> &OnValue() { return m_onValue; }

protected:
	std::string GetTextString() const override
	{
		return String::To(static_cast<int32_t>(m_value));
	}

private:
	MouseButton m_value;
	Delegate<void(MouseButton)> m_onValue;
};
}
