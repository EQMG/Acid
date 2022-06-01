#include "UiSection.hpp"

#include "Uis/Uis.hpp"

namespace acid {
UiSection::UiSection() {
	icon.SetImage(Image2d::Create("Guis/Triangle_Down.png"));
	UiObject::AddChild(&icon);

	title.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	title.SetTextColour(Colour::White);
	UiObject::AddChild(&title);
	
	UiObject::AddChild(&content);

	OnClick().connect(this, [this](MouseButton button) {
		if (button == MouseButton::Left) {
			CancelEvent(MouseButton::Left);

			collapsed = !collapsed;

			if (collapsed) {
				icon.SetImage(Image2d::Create("Guis/Triangle_Right.png"));
			} else {
				icon.SetImage(Image2d::Create("Guis/Triangle_Down.png"));
			}

			onCollapsed(this, collapsed);
		}
	});
}

void UiSection::UpdateObject() {
	content.SetEnabled(!collapsed);
}

void UiSection::AddChild(UiObject *child) {
	content.AddChild(child);
}
}
