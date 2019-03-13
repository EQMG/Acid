#pragma once

#include "Audio/Sound.hpp"
#include "Devices/Joysticks.hpp"
#include "Devices/Keyboard.hpp"
#include "Devices/Mouse.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputGrabber : public UiObject
{
  public:
	UiInputGrabber(UiObject* parent, const std::string& title, const UiBound& rectangle);

	virtual ~UiInputGrabber() = default;

	void UpdateObject() override;

	void SetSelected(const bool& selected);

	const std::string& GetTitle() const
	{
		return m_title;
	}

	void SetTitle(const std::string& title);

  protected:
	void UpdateText();

	virtual std::string GetTextString() const = 0;

	Gui m_background;
	Text m_textTitle;
	Text m_textValue;
	Sound m_soundClick;

	std::string m_title;

	int32_t m_lastKey;

	bool m_selected;
	bool m_mouseOver;
};

class ACID_EXPORT UiGrabberJoystick : public UiInputGrabber
{
  public:
	UiGrabberJoystick(UiObject* parent, const std::string& title, const uint32_t& port, const uint32_t& value, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	const uint32_t& GetPort() const
	{
		return m_port;
	}

	void SetPort(const uint32_t& port)
	{
		m_port = port;
	}

	const uint32_t& GetValue() const
	{
		return m_value;
	}

	void SetValue(const uint32_t& value)
	{
		m_value = value;
		UpdateText();
	}

	Delegate<void(UiGrabberJoystick*, uint32_t, uint32_t)>& GetOnGrabbed()
	{
		return m_onGrabbed;
	}

  protected:
	std::string GetTextString() const override
	{
		return String::To(m_value);
	}

  private:
	uint32_t m_port;
	uint32_t m_value;
	Delegate<void(UiGrabberJoystick*, uint32_t, uint32_t)> m_onGrabbed;
};

class ACID_EXPORT UiGrabberKeyboard : public UiInputGrabber
{
  public:
	UiGrabberKeyboard(UiObject* parent, const std::string& title, const Key& value, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	const Key& GetValue() const
	{
		return m_value;
	}

	void SetValue(const Key& value)
	{
		m_value = value;
		UpdateText();
	}

	Delegate<void(UiGrabberKeyboard*, Key)>& GetOnGrabbed()
	{
		return m_onGrabbed;
	}

  protected:
	std::string GetTextString() const override
	{
		return Keyboard::ToString(m_value);
	}

  private:
	Key m_value;
	Delegate<void(UiGrabberKeyboard*, Key)> m_onGrabbed;
};

class ACID_EXPORT UiGrabberMouse : public UiInputGrabber
{
  public:
	UiGrabberMouse(UiObject* parent, const std::string& title, const MouseButton& value, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	const MouseButton& GetValue() const
	{
		return m_value;
	}

	void SetValue(const MouseButton& value)
	{
		m_value = value;
		UpdateText();
	}

	Delegate<void(UiGrabberMouse*, MouseButton)>& GetOnGrabbed()
	{
		return m_onGrabbed;
	}

  protected:
	std::string GetTextString() const override
	{
		return String::To(static_cast<int32_t>(m_value));
	}

  private:
	MouseButton m_value;
	Delegate<void(UiGrabberMouse*, MouseButton)> m_onGrabbed;
};
}
