#include "Uis.hpp"

#include "Constraints/PixelConstraint.hpp"

namespace acid {
Uis::Uis() {
	canvas.GetConstraints().SetWidth<PixelConstraint>(0)
		.SetHeight<PixelConstraint>(0);
	for (auto button : EnumIterator<MouseButton>())
		selectors.emplace(button, SelectorMouse());
}

void Uis::Update() {
	for (auto &[button, selector] : selectors) {
		auto isDown = Mouse::Get()->GetButton(button) != InputAction::Release;
		selector.wasDown = !selector.isDown && isDown;
		selector.isDown = isDown;
	}

	auto lastCursorSelect = cursorSelect;
	cursorSelect = nullptr;

	objects.clear();
	auto viewMatrix = Matrix4::OrthographicMatrix(0.0f, Window::Get()->GetSize().x, 
		0.0f, Window::Get()->GetSize().y, -1.0f, 1.0f);
	canvas.GetConstraints().GetWidth()->SetOffset(Window::Get()->GetSize().x);
	canvas.GetConstraints().GetHeight()->SetOffset(Window::Get()->GetSize().y);
	canvas.Update(viewMatrix, objects, cursorSelect);

	if (lastCursorSelect != cursorSelect) {
		Mouse::Get()->SetCursor(cursorSelect ? *cursorSelect->GetCursorHover() : CursorStandard::Arrow);
	}
}

bool Uis::IsDown(MouseButton button) {
	return selectors[button].isDown;
}

bool Uis::WasDown(MouseButton button) {
	return selectors[button].wasDown;
}

void Uis::CancelWasEvent(MouseButton button) {
	selectors[button].wasDown = false;
}
}
