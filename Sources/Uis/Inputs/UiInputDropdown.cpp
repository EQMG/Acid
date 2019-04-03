#include "UiInputDropdown.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"
#include "UiInputButton.hpp"

namespace acid
{
UiInputDropdown::UiInputDropdown(UiObject *parent, const std::string &title, const uint32_t &value, const std::vector<std::string> &options, const UiBound &rectangle) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Maximum, Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, UiBound(Vector2f(1.0f - (2.5f * UiInputButton::Padding.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Size),
		UiInputButton::FontSize, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)
{
	GetRectangle().SetSize(UiInputButton::Size);
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.75f, 0.75f));

	OnSelected() += [this](bool selected)
	{
		m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour, 
			UiInputButton::SlideTime));
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	};
	OnClick() += [this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			if (!m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			CancelEvent(MouseButton::Left);
		}
	};
}

void UiInputDropdown::UpdateObject()
{
}
}
