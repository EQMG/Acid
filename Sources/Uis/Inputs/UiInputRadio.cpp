#include "UiInputRadio.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"
#include "UiInputButton.hpp"

namespace acid
{
static const Vector2f SIZE = Vector2f(0.22f, 0.0315f);

UiInputRadio::UiInputRadio(UiObject *parent, const std::string &string, const Type &type, const bool &value, const UiBound &rectangle) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Left, Image2d::Create("Guis/Radio.png"), UiInputButton::PrimaryColour),
	m_fill(&m_background, UiBound::Maximum, nullptr, Colour::White),
	m_text(this, UiBound::Left, UiInputButton::FontSize, string, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f),
	m_value(value),
	m_type(type)
{
	GetRectangle().SetSize(SIZE);
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.75f, 0.75f));
	m_background.GetRectangle().SetSize(Vector2f(GetRectangle().GetSize().m_y));
	m_text.GetRectangle().SetPosition(Vector2f(5.4f * GetRectangle().GetSize().m_y, 0.5f));
	UpdateValue();

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
			m_value = !m_value;
			UpdateValue();
			m_onValue(m_value);
		}
	};
}

void UiInputRadio::UpdateObject()
{
}

void UiInputRadio::SetValue(const bool &value)
{
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}

void UiInputRadio::SetType(const Type &type)
{
	m_type = type;
	UpdateValue();
}

void UiInputRadio::UpdateValue()
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
