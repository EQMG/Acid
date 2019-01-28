#include "UiInputRadio.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"
#include "UiInputButton.hpp"

namespace acid
{
	static const Time SLIDE_TIME = Time::Seconds(0.1f);
	static const Vector2 SIZE = Vector2(0.3f, 0.03375f);
	static const float FONT_SIZE = 1.4f;

	UiInputRadio::UiInputRadio(UiObject *parent, const std::string &string, const Mark &markType, const bool &checked, const UiBound &rectangle) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Left, Texture::Create("Guis/Radio.png"), UiInputButton::PrimaryColour)),
		m_fill(std::make_unique<Gui>(m_background.get(), UiBound::Maximum, nullptr, Colour::White)),
		m_text(std::make_unique<Text>(this, UiBound::Left, FONT_SIZE, string,
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_checked(checked),
		m_markType(markType),
		m_mouseOver(false),
		m_onChecked(Delegate<void(UiInputRadio *, bool)>())
	{
		GetRectangle().SetDimensions(SIZE);
		m_background->SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));

		m_background->GetRectangle().SetDimensions(Vector2(GetRectangle().GetDimensions().m_y, GetRectangle().GetDimensions().m_y));
		m_text->GetRectangle().SetPosition(Vector2(4.0f * GetRectangle().GetDimensions().m_y, 0.5f));

		UpdateFill();
	}

	void UiInputRadio::UpdateObject()
	{
		if (m_background->IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
		{
			if (!m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			m_checked = !m_checked;
			m_onChecked(this, m_checked);
			m_fill->SetAlphaDriver<DriverSlide<float>>(m_fill->GetAlpha(), m_checked ? 1.0f : 0.0f, SLIDE_TIME);
			CancelEvent(MouseButton::Left);
		}

		if (m_background->IsSelected() && !m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), UiInputButton::SelectedColour, SLIDE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), UiInputButton::PrimaryColour, SLIDE_TIME);
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
			case Mark::Filled:
				m_fill->SetTexture(Texture::Create("Guis/Radio_Filled.png"));
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
