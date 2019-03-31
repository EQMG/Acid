#include "UiInputRadio.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"
#include "UiInputButton.hpp"

namespace acid
{
static const Vector2 SIZE = Vector2(0.22f, 0.0315f);

UiInputRadio::UiInputRadio(UiObject *parent, const std::string &string, const Type &type, const bool &value, const UiBound &rectangle) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Left, Image2d::Create("Guis/Radio.png"), UiInputButton::PrimaryColour),
	m_fill(&m_background, UiBound::Maximum, nullptr, Colour::White),
	m_text(this, UiBound::Left, UiInputButton::FontSize, string, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f),
	m_value(value),
	m_type(type),
	m_mouseOver(false)
{
	GetRectangle().SetDimensions(SIZE);
	m_background.SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));

	m_background.GetRectangle().SetDimensions(Vector2(GetRectangle().GetDimensions().m_y, GetRectangle().GetDimensions().m_y));
	m_text.GetRectangle().SetPosition(Vector2(5.4f * GetRectangle().GetDimensions().m_y, 0.5f));

	UpdateFill();
}

void UiInputRadio::UpdateObject()
{
	if (m_background.IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
	{
		if (!m_soundClick.IsPlaying())
		{
			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

		m_value = !m_value;
		m_onValue(m_value);
		m_fill.SetAlphaDriver(new DriverSlide<float>(m_fill.GetAlpha(), m_value ? 1.0f : 0.0f, UiInputButton::SlideTime));
		CancelEvent(MouseButton::Left);
	}

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

void UiInputRadio::SetValue(const bool &value)
{
	m_value = value;
	UpdateFill();
	//m_onValue(m_value);
}

void UiInputRadio::SetType(const Type &type)
{
	m_type = type;
	UpdateFill();
}

void UiInputRadio::UpdateFill()
{
	switch (m_type)
	{
	case Type::Filled:
		m_fill.SetTexture(Image2d::Create("Guis/Radio_Filled.png"));
		break;
	case Type::X:
		m_fill.SetTexture(Image2d::Create("Guis/Radio_X.png"));
		break;
	case Type::Check:
		m_fill.SetTexture(Image2d::Create("Guis/Radio_Check.png"));
		break;
	case Type::Dot:
		m_fill.SetTexture(Image2d::Create("Guis/Radio_Dot.png"));
		break;
	}

	m_fill.SetAlphaDriver(new DriverSlide<float>(m_fill.GetAlpha(), m_value ? 1.0f : 0.0f, UiInputButton::SlideTime));
}
}
