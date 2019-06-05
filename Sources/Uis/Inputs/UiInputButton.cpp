#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
const Vector2i UiInputButton::Size = Vector2f(150, 22);
const Vector2i UiInputButton::Padding = Vector2i(5, 5);
const float UiInputButton::FontSize = 11.0f;
const Time UiInputButton::SlideTime = 0.07s;

const Colour UiInputButton::ValueColour = Colour("#FEFCFE");
const Colour UiInputButton::TitleColour = Colour("#9C9A9C");

const Colour UiInputButton::BackgroundColour = Colour("#282729");
const Colour UiInputButton::PrimaryColour = Colour("#121113");
const Colour UiInputButton::SelectedColour = Colour("#FEA62A");
const Colour UiInputButton::ButtonColour = Colour("#3C3B3C");

UiInputButton::UiInputButton(UiObject *parent, const std::string &string, const UiTransform &transform) :
	UiObject(parent, transform),
	m_background(this, UiTransform(UiMargins::All), Image2d::Create("Guis/Button_Filled.png"), ButtonColour),
	m_text(this, UiTransform(UiMargins::None, Padding, -Padding), FontSize, string,
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, ValueColour)
{
	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected)
	{
		m_background.SetColourDriver(std::make_unique<DriverSlide<Colour>>(m_background.GetColourOffset(), selected ? SelectedColour : ButtonColour, SlideTime));
	});

	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
}

void UiInputButton::UpdateObject()
{
}
}
