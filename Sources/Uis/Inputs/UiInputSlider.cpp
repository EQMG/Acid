#include "UiInputSlider.hpp"

#include <iomanip>
#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"
#include "UiInputButton.hpp"

namespace acid
{
static const Time CHANGE_TIME = 0.05s;

UiInputSlider::UiInputSlider(UiObject *parent, const std::string &title, const float &value, const float &valueMin, const float &valueMax, const int32_t &roundTo,
	const UiTransform &rectangle) :
	UiObject(parent, rectangle),
	m_slider(this, UiTransform(Vector2f(0.0f, 0.5f), UiAnchor::CentreLeft, UiAspect::Position | UiAspect::Scale), Image2d::Create("Guis/Button_Filled.png"),
		UiInputButton::PrimaryColour),
	m_background(this, UiTransform::Maximum, Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, UiTransform(Vector2f(1.0f - (2.5f * UiInputButton::Padding.m_x), 0.5f), UiAnchor::CentreRight, UiAspect::Position | UiAspect::Size), UiInputButton::FontSize,
		title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, UiInputButton::TitleColour),
	m_textValue(this, UiTransform(Vector2f(2.5f * UiInputButton::Padding.m_x, 0.5f), UiAnchor::CentreLeft, UiAspect::Position | UiAspect::Size), UiInputButton::FontSize, "",
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, UiInputButton::ValueColour),
	m_value(value),
	m_valueMin(valueMin),
	m_valueMax(valueMax),
	m_progress(0.0f),
	m_roundTo(roundTo),
	m_updating(false),
	m_mouseOver(false),
	m_hasChange(false),
	m_timerChange(CHANGE_TIME)
{
	GetTransform().SetSize(UiInputButton::Size);
	m_slider.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
	UpdateProgress();

	OnSelected().Add([this](bool selected)
	{
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	});
}

void UiInputSlider::UpdateObject()
{
	if (m_background.IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
	{
		m_updating = true;
		CancelEvent(MouseButton::Left);
	}
	else if (!Uis::Get()->IsDown(MouseButton::Left))
	{
		m_updating = false;
	}
	else if (m_updating)
	{
		float width = m_background.GetScreenSize().m_x;
		float positionX = m_background.GetScreenPosition().m_x;
		float cursorX = Mouse::Get()->GetPosition().m_x - positionX;
		m_progress = cursorX / width;
		m_progress = std::clamp(m_progress, 0.0f, 1.0f);
		m_value = (m_progress * (m_valueMax - m_valueMin)) + m_valueMin;
		m_onValue(m_value);

		m_hasChange = true;
		CancelEvent(MouseButton::Left);
	}

	if (m_hasChange && m_timerChange.IsPassedTime())
	{
		m_timerChange.ResetStartTime();
		m_hasChange = false;
		UpdateProgress();
	}

	if (m_background.IsSelected() && !m_mouseOver)
	{
		m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), UiInputButton::SelectedColour, UiInputButton::SlideTime));
		m_mouseOver = true;
	}
	else if (!m_background.IsSelected() && m_mouseOver && !m_updating)
	{
		m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), UiInputButton::PrimaryColour, UiInputButton::SlideTime));
		m_mouseOver = false;
	}

	m_slider.GetTransform().SetSize(Vector2f(m_progress, 1.0f));
}

void UiInputSlider::SetValue(const float &value)
{
	m_value = value;
	UpdateProgress();
	//m_onValue(m_value);
}

void UiInputSlider::UpdateProgress()
{
	m_progress = (m_value - m_valueMin) / (m_valueMax - m_valueMin);
	std::stringstream rounded;
	rounded << std::fixed << std::setprecision(m_roundTo) << m_value;
	m_textValue.SetString(rounded.str());
}
}
