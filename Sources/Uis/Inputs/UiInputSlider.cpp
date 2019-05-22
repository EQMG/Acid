#include "UiInputSlider.hpp"

#include <iomanip>
#include "Maths/Visual/DriverConstant.hpp"
#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
UiInputSlider::UiInputSlider(UiObject *parent, const std::string &title, const float &value, const float &valueMin, const float &valueMax, const int32_t &roundTo,
	const UiTransform &transform) :
	UiObject(parent, transform),
	m_slider(this, UiTransform(transform.GetSize(), UiAnchor::LeftTop), Image2d::Create("Guis/Button_Filled.png"),
		UiInputButton::PrimaryColour),
	m_background(this, UiTransform(transform.GetSize(), UiAnchor::Centre), Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, UiTransform(transform.GetSize() - (2 * UiInputButton::Padding), UiAnchor::RightCentre, Vector2i(-UiInputButton::Padding, 0)), UiInputButton::FontSize,
		title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, UiInputButton::TitleColour),
	m_textValue(this, UiTransform(transform.GetSize() - (2 * UiInputButton::Padding), UiAnchor::LeftCentre, Vector2i(UiInputButton::Padding, 0)), UiInputButton::FontSize, "",
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, UiInputButton::ValueColour),
	m_value(value),
	m_valueMin(valueMin),
	m_valueMax(valueMax),
	m_progress(0.0f),
	m_roundTo(roundTo),
	m_updating(false),
	m_mouseOver(false)
{
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
		auto width = m_background.GetScreenTransform().GetSize().m_x;
		auto positionX = m_background.GetScreenTransform().GetPosition().m_x;
		auto cursorX = static_cast<float>(Mouse::Get()->GetPosition().m_x) - positionX;
		m_progress = cursorX / width;
		m_progress = std::clamp(m_progress, 0.0f, 1.0f);
		m_value = (m_progress * (m_valueMax - m_valueMin)) + m_valueMin;
		m_onValue(m_value);

		CancelEvent(MouseButton::Left);
	}

	UpdateProgress();

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

	m_slider.GetTransform().SetSize(Vector2f(m_progress, 1.0f) * GetTransform().GetSize());
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
