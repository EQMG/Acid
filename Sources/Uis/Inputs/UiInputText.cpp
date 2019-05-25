﻿#include "UiInputText.hpp"
#include <utility>
#include "Devices/Keyboard.hpp"
#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
UiInputText::UiInputText(UiObject *parent, const std::string &title, std::string value, const int32_t &maxLength, const UiTransform &transform) :
	UiObject(parent, transform),
	m_background(this, UiTransform(UiMargins::All), Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, UiTransform(UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding), UiInputButton::FontSize,
		title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Right, UiInputButton::TitleColour),
	m_textValue(this, UiTransform(UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding), UiInputButton::FontSize, value,
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, UiInputButton::ValueColour),
	m_value(std::move(value)),
	m_maxLength(maxLength),
	m_lastKey(0),
	m_updating(false),
	m_mouseOver(false)
{
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));

	Keyboard::Get()->OnKey().Add([this](Key key, InputAction action, BitMask<InputMod> mods)
	{
		if (!m_updating)
		{
			return;
		}

		if (key == Key::Backspace && action != InputAction::Release)
		{
			m_inputDelay.Update(true);

			if (m_lastKey != 8 || m_inputDelay.CanInput())
			{
				m_value = m_value.substr(0, m_value.length() - 1);
				m_textValue.SetString(m_value);
				m_onValue(m_value);
				m_lastKey = 8;
			}
		}
		else if (key == Key::Enter && action != InputAction::Release && m_lastKey != 13)
		{
			m_inputDelay.Update(true);
			SetUpdating(false);
		}
	}, this);
	Keyboard::Get()->OnChar().Add([this](char c)
	{
		if (!m_updating)
		{
			return;
		}

		if (m_value.length() < static_cast<uint32_t>(m_maxLength))
		{
			m_inputDelay.Update(true);

			if (m_lastKey != c || m_inputDelay.CanInput())
			{
				m_value += c;
				m_textValue.SetString(m_value);
				m_onValue(m_value);
				m_lastKey = c;
			}
		}
		else
		{
			m_inputDelay.Update(false);
			m_lastKey = 0;
		}
	}, this);

	OnSelected().Add([this](bool selected)
	{
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	});
}

void UiInputText::UpdateObject()
{
	if (Uis::Get()->WasDown(MouseButton::Left))
	{
		if (m_background.IsSelected())
		{
			SetUpdating(true);
			CancelEvent(MouseButton::Left);
		}
		else if (m_updating)
		{
			SetUpdating(false);
			CancelEvent(MouseButton::Left);
		}
	}

	if (!m_updating)
	{
		if (m_background.IsSelected() && !m_mouseOver)
		{
			m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), UiInputButton::SelectedColour, UiInputButton::SlideTime));
			m_mouseOver = true;
		}
		else if (!m_background.IsSelected() && m_mouseOver)
		{
			m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), UiInputButton::PrimaryColour, UiInputButton::SlideTime));
			m_mouseOver = false;
		}
	}
}

void UiInputText::SetUpdating(const bool &updating)
{
	m_updating = updating;
	m_mouseOver = true;
}

void UiInputText::SetValue(const std::string &value)
{
	m_value = value;
	m_textValue.SetString(value);
	//m_onValue(m_value);
}
}
