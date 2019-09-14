#include "UiSection.hpp"

#include "Uis/Uis.hpp"

namespace acid
{
UiSection::UiSection(UiObject *parent, const std::string &string, const UiTransform &transform) :
	UiObject(parent, transform),
	m_icon(this, transform, Image2d::Create("Guis/Triangle_Down.png")),
	m_text(this, transform, 12, string, FontType::Create("Fonts/ProximaNova"), Text::Justify::Left, Colour::White),
	m_content(this, transform)
{
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
	m_content.SetTransform(GetTransform());
	m_content.SetEnabled(!m_collapsed);
}
}
