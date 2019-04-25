#include "UiSection.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
static const Time SLIDE_TIME = Time::Seconds(0.1f);
static const Vector2f SIZE = Vector2f(0.3f, 0.03375f);
static const float FONT_SIZE = 1.4f;

UiSection::UiSection(UiObject *parent, const std::string &string, const UiBound &rectangle) :
	UiObject(parent, rectangle),
	m_icon(this, UiBound::Left, Image2d::Create("Guis/Triangle_Down.png")),
	m_text(this, UiBound::Left, FONT_SIZE, string, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White),
	m_content(this, UiBound(Vector2f(0.0f, 4.0f * SIZE.m_x), UiReference::TopLeft, UiAspect::Position | UiAspect::Size)),
	m_collapsed(false)
{
	GetRectangle().SetSize(SIZE);
	m_icon.GetRectangle().SetSize(Vector2f(GetRectangle().GetSize().m_y));
	m_text.GetRectangle().SetPosition(Vector2f(4.0f * GetRectangle().GetSize().m_y, 0.5f));

	OnClick().Add([this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			CancelEvent(MouseButton::Left);

			m_collapsed = !m_collapsed;

			if (m_collapsed)
			{
				m_icon.SetImage(Image2d::Create("Guis/Triangle_Right.png"));
			}
			else
			{
				m_icon.SetImage(Image2d::Create("Guis/Triangle_Down.png"));
			}

			m_onCollapsed(this, m_collapsed);
		}
	});
}

void UiSection::UpdateObject()
{
	m_content.SetEnabled(!m_collapsed);
}
}
