#include "Uis.hpp"

namespace acid {
Uis::Uis() {
	m_canvas.SetTransform({Window::Get()->GetSize()});
	for (auto button : EnumIterator<MouseButton>()) {
		m_selectors.emplace(button, SelectorMouse());
	}
}

void Uis::Update() {
	for (auto &[button, selector] : m_selectors) {
		auto isDown = Mouse::Get()->GetButton(button) != InputAction::Release;
		selector.m_wasDown = !selector.m_isDown && isDown;
		selector.m_isDown = isDown;
	}

	auto lastCursorSelect = m_cursorSelect;
	m_cursorSelect = nullptr;

	auto viewMatrix = Matrix4::OrthographicMatrix(0.0f, Window::Get()->GetSize().m_x, 
		0.0f, Window::Get()->GetSize().m_y, -1.0f, 1.0f);
	m_objects.clear();
	m_canvas.GetTransform().SetSize(Window::Get()->GetSize());
	m_canvas.Update(viewMatrix, m_objects, m_cursorSelect);

	if (lastCursorSelect != m_cursorSelect) {
		Mouse::Get()->SetCursor(m_cursorSelect ? *m_cursorSelect->GetCursorHover() : CursorStandard::Arrow);
	}
}

bool Uis::IsDown(MouseButton button) {
	return m_selectors[button].m_isDown;
}

bool Uis::WasDown(MouseButton button) {
	return m_selectors[button].m_wasDown;
}

void Uis::CancelWasEvent(MouseButton button) {
	m_selectors[button].m_wasDown = false;
}
}
