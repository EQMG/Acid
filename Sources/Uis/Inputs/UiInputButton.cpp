#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
const Vector2i UiInputButton::Size = Vector2f(150, 30);
const int32_t UiInputButton::Padding = 5;
const float UiInputButton::FontSize = 12.0f;
const Time UiInputButton::SlideTime = 0.1s;

const Colour UiInputButton::TitleColour = Colour("#8da2a5");
const Colour UiInputButton::ValueColour = Colour("#e4e6e7");

const Colour UiInputButton::BackgroundColour = Colour("#364346");
const Colour UiInputButton::PrimaryColour = Colour("#2c393c");
const Colour UiInputButton::SelectedColour = Colour("#20292B");

UiInputButton::UiInputButton(UiObject *parent, const std::string &string, const UiTransform &transform) :
	UiObject(parent, transform),
	m_background(this, UiTransform(transform.GetSize(), UiAnchor::Centre), Image2d::Create("Guis/Button_Filled.png"), PrimaryColour),
	m_text(this, UiTransform(transform.GetSize() - (2 * Padding), UiAnchor::Centre), FontSize, string, 
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, ValueColour)
{
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
