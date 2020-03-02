#include "UiScrollBar.hpp"

#include "Inputs/UiInputButton.hpp"
#include "Drivers/ConstantDriver.hpp"
#include "Drivers/SlideDriver.hpp"
#include "Uis.hpp"

namespace acid {
UiScrollBar::UiScrollBar() {
	//background.SetTransform({UiMargins::All});
	background.SetImage(Image2d::Create("Guis/White.png"));
	background.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&background);

	//scroll.SetTransform({UiMargins::RightBottom});
	scroll.SetImage(Image2d::Create("Guis/White.png"));
	scroll.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&scroll);

	Mouse::Get()->OnScroll().Add([this](Vector2d wheelDelta) {
		if (GetParent()->IsSelected() && !updating && scroll.IsEnabled()) {
			Vector2f position;
			position[index] = ScrollByDelta(wheelDelta[index]);
//			scroll.GetTransform().SetPosition(position);
		}
	}, this);
}

void UiScrollBar::UpdateObject() {
	if (scroll.IsSelected() && Uis::Get()->WasDown(MouseButton::Left)) {
		updating = true;
	} else if (updating) {
		if (!Uis::Get()->IsDown(MouseButton::Left)) {
			updating = false;
		}

		Vector2d position;
		position[index] = Mouse::Get()->GetPosition()[index] - GetScreenPosition()[index]; // ScrollByPosition(Mouse::Get()->GetPosition()[index]);
//		scroll.GetTransform().SetPosition(position);
		CancelEvent(MouseButton::Left);
	}

	if (!updating) {
		if (scroll.IsSelected() && !mouseOver) {
			scroll.SetColourDriver<SlideDriver>(scroll.GetColourDriver()->Get(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
			mouseOver = true;
		} else if (!scroll.IsSelected() && mouseOver) {
			scroll.SetColourDriver<SlideDriver>(scroll.GetColourDriver()->Get(), UiInputButton::ButtonColour, UiInputButton::SlideTime);
			mouseOver = false;
		}
	}
}

float UiScrollBar::GetProgress() {
	// TODO: Mark const
//	return scroll.GetTransform().GetPosition()[index];
	return 0.0f;
}

void UiScrollBar::SetSize(const Vector2f &size) {
//	scroll.GetTransform().SetAnchor0(scroll.GetTransform().GetPosition());
//	scroll.GetTransform().SetAnchor1(scroll.GetTransform().GetPosition() + size);
}

void UiScrollBar::SetType(ScrollBar type) {
	index = type == ScrollBar::Vertical;
}

float UiScrollBar::ScrollByDelta(float delta) const {
	float puckLength = scroll.GetScreenSize()[index];
	float barLength = GetParent()->GetScreenSize()[index];
	auto maxValue = (barLength - puckLength) / barLength;
	float value = scroll.GetScreenPosition()[index];
	value += delta;
	return std::clamp(value, 0.0f, maxValue);
}

float UiScrollBar::ScrollByPosition(float position) const {
	float puckLength = scroll.GetScreenSize()[index];
	float barLength = GetParent()->GetScreenSize()[index];
	auto maxValue = (barLength - puckLength) / barLength;
	float positionLength = GetParent()->GetScreenPosition()[index];
	auto cursorLength = (position - positionLength) - (puckLength / 2.0f);
	return std::clamp(cursorLength / barLength, 0.0f, maxValue);
}
}
