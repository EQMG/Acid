#include "UiSection.hpp"

#include "Uis/Uis.hpp"

namespace acid {
UiSection::UiSection() {
	m_icon.SetImage(Image2d::Create("Guis/Triangle_Down.png"));
	AddChild(&m_icon);

	m_title.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_title.SetTextColour(Colour::White);
	AddChild(&m_title);
	
	AddChild(&m_content);

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
	});
}

void UiSection::UpdateObject() {
	m_content.SetEnabled(!m_collapsed);
}

void UiSection::SetTransform(const UiTransform &transform) {
	m_icon.SetTransform(transform);
	m_title.SetTransform(transform);
	m_content.SetTransform(transform);
}
}
