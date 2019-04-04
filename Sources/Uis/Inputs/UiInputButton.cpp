#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
const Vector2f UiInputButton::Size = Vector2f(0.22f, 0.042f);
const Vector2f UiInputButton::Padding = Vector2f(0.01f, 0.07f);
const float UiInputButton::FontSize = 1.3f;
const Time UiInputButton::SlideTime = Time::Seconds(0.12f);

const Colour UiInputButton::PrimaryColour = Colour("#474747");
const Colour UiInputButton::BackgroundColour = Colour("#282828");
const Colour UiInputButton::AccentColour = Colour("#1e88e5");
const Colour UiInputButton::SelectedColour = Colour("#1976d2");

UiInputButton::UiInputButton(UiObject *parent, const std::string &string, const UiBound &rectangle) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Maximum, Image2d::Create("Guis/Button_Filled.png"), AccentColour),
	m_text(this, UiBound::Centre, FontSize, string, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)
{
	GetRectangle().SetSize(Size);
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));

	OnSelected() += [this](bool selected)
	{
		m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), selected ? SelectedColour : AccentColour, 
			SlideTime));
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

void UiInputButton::UpdateObject()
{
}
}
