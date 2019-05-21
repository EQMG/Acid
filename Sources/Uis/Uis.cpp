#include "Uis.hpp"

namespace acid
{
Uis::Uis() :
	m_canvas(nullptr, UiTransform(Window::Get()->GetSize()))
{
	for (auto button : EnumIterator<MouseButton>())
	{
		m_selectors.emplace(button, SelectorMouse());
	}
}

void Uis::Update()
{
	for (auto &[button, selector] : m_selectors)
	{
		bool isDown = Mouse::Get()->GetButton(button) != InputAction::Release;
		selector.m_wasDown = !selector.m_isDown && isDown;
		selector.m_isDown = isDown;
	}

	auto viewMatrix = Matrix4::OrthographicMatrix(0.0f, Window::Get()->GetSize().m_x, 0.0f, Window::Get()->GetSize().m_y, 0.0f, 1.0f);
	m_objects.clear();
	m_canvas.GetTransform().SetSize(Window::Get()->GetSize());
	m_canvas.Update(viewMatrix, m_objects);
}

void Uis::CancelWasEvent(const MouseButton &button)
{
	m_selectors[button].m_wasDown = false;
}

bool Uis::IsDown(const MouseButton &button)
{
	return m_selectors[button].m_isDown;
}

bool Uis::WasDown(const MouseButton &button)
{
	return m_selectors[button].m_wasDown;
}
}
