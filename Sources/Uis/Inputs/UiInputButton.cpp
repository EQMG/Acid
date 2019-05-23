#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
const Vector2i UiInputButton::Size = Vector2f(175, 26);
const int32_t UiInputButton::Padding = 5;
const float UiInputButton::FontSize = 12.0f;
const Time UiInputButton::SlideTime = 0.1s;

const Colour UiInputButton::ValueColour = Colour("#FEFCFE");
const Colour UiInputButton::TitleColour = Colour("#9C9A9C");

const Colour UiInputButton::BackgroundColour = Colour("#282729");
const Colour UiInputButton::PrimaryColour = Colour("#121113");
const Colour UiInputButton::SelectedColour = Colour("#FEA62A");
const Colour UiInputButton::ButtonColour = Colour("#3C3B3C");

UiInputButton::UiInputButton(UiObject *parent, const std::string &string, const UiTransform &transform) :
	UiObject(parent, transform),
	m_background(this, UiTransform(transform.GetSize(), UiAnchor::Centre), Image2d::Create("Guis/Button_Filled.png"), ButtonColour),
	m_text(this, UiTransform(transform.GetSize() - (2 * Padding), UiAnchor::Centre), FontSize, string, 
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, ValueColour)
{
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));

	OnSelected().Add([this](bool selected)
	{
		m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), selected ? SelectedColour : ButtonColour, SlideTime));
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	});
}

void UiInputButton::UpdateObject()
{
}
}
