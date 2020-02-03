#include "UiSection.hpp"

#include "Uis/Uis.hpp"

namespace acid {
UiSection::UiSection() {
	m_icon.SetImage(Image2d::Create("Guis/Triangle_Down.png"));
	UiObject::AddChild(&m_icon);

	m_title.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_title.SetTextColour(Colour::White);
	UiObject::AddChild(&m_title);
	
	UiObject::AddChild(&m_content);

	OnClick().Add([this](MouseButton button) {
		if (button == MouseButton::Left) {
			CancelEvent(MouseButton::Left);

			m_collapsed = !m_collapsed;

			if (m_collapsed) {
				m_icon.SetImage(Image2d::Create("Guis/Triangle_Right.png"));
			} else {
				m_icon.SetImage(Image2d::Create("Guis/Triangle_Down.png"));
			}

			m_onCollapsed(this, m_collapsed);
		}
	}, this);
}

void UiSection::UpdateObject() {
	m_content.SetEnabled(!m_collapsed);
}

void UiSection::AddChild(UiObject *child) {
	m_content.AddChild(child);
}
}
