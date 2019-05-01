#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
const Vector2f UiInputButton::Size = Vector2f(0.18f, 0.036f);
const Vector2f UiInputButton::Padding = Vector2f(0.01f, 0.07f);
const float UiInputButton::FontSize = 1.2f;
const Time UiInputButton::SlideTime = Time::Seconds(0.1f);

const Colour UiInputButton::TitleColour = Colour("#8da2a5");
const Colour UiInputButton::ValueColour = Colour("#e4e6e7");

const Colour UiInputButton::BackgroundColour = Colour("#364346");
const Colour UiInputButton::PrimaryColour = Colour("#2c393c");
const Colour UiInputButton::SelectedColour = Colour("#20292B");

UiInputButton::UiInputButton(UiObject *parent, const std::string &string, const UiBound &rectangle) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Maximum, Image2d::Create("Guis/Button_Filled.png"), PrimaryColour),
	m_text(this, UiBound::Centre, FontSize, string, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, ValueColour)
{
	GetRectangle().SetSize(Size);
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));

	OnSelected().Add([this](bool selected)
	{
		m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), selected ? SelectedColour : PrimaryColour, SlideTime));
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	});
}

void UiInputButton::UpdateObject()
{
}
}
