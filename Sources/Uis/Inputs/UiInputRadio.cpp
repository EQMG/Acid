#include "UiInputRadio.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	static const Time SLIDE_TIME = Time::Seconds(0.1f);
	static const Vector2 SIZE = Vector2(0.3f, 0.03375f);
	static const float FONT_SIZE = 1.4f;

	UiInputRadio::UiInputRadio(UiObject *parent, const std::string &string, const Mark &markType, const bool &checked, const UiBound &rectangle, const Colour &primaryColour) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Left, Texture::Create("Guis/Radio.png"), primaryColour)),
		m_fill(std::make_unique<Gui>(m_background.get(), UiBound::Maximum, nullptr, Colour::White)),
		m_text(std::make_unique<Text>(this, UiBound::Left, FONT_SIZE, string,
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_primaryColour(primaryColour),
		m_checked(checked),
		m_markType(markType),
		m_mouseOver(false),
		m_onChecked(Delegate<void(UiInputRadio *, bool)>())
	{
		GetRectangle().SetDimensions(SIZE);
		m_background->GetRectangle().SetDimensions(Vector2(SIZE.m_y, SIZE.m_y));
		m_text->GetRectangle().SetPosition(Vector2(4.0f * SIZE.m_y, 0.5f));
		UpdateFill();
	}

	void UiInputRadio::UpdateObject()
	{
		if (m_background->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			if (!m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			m_checked = !m_checked;
			m_fill->SetAlphaDriver<DriverSlide<float>>(m_fill->GetAlpha(), m_checked ? 1.0f : 0.0f, SLIDE_TIME);
			m_onChecked(this, m_checked);
		}

		if (m_background->IsSelected() && !m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), 1.3f * m_primaryColour, SLIDE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
			m_mouseOver = false;
		}
	}

	void UiInputRadio::SetChecked(const bool &checked)
	{
		m_checked = checked;
		UpdateFill();
	}

	void UiInputRadio::SetMarkType(const UiInputRadio::Mark &markType)
	{
		m_markType = markType;
		UpdateFill();
	}

	void UiInputRadio::UpdateFill()
	{
		switch(m_markType)
		{
			case Mark::Fill:
				m_fill->SetTexture(Texture::Create("Guis/Radio_Full.png"));
				break;
			case Mark::X:
				m_fill->SetTexture(Texture::Create("Guis/Radio_X.png"));
				break;
			case Mark::Check:
				m_fill->SetTexture(Texture::Create("Guis/Radio_Check.png"));
				break;
			case Mark::Dot:
				m_fill->SetTexture(Texture::Create("Guis/Radio_Dot.png"));
				break;
			default:
				m_fill->SetTexture(nullptr);
				break;
		}

		m_fill->SetAlphaDriver<DriverSlide<float>>(m_fill->GetAlpha(), m_checked ? 1.0f : 0.0f, SLIDE_TIME);
	}
}
