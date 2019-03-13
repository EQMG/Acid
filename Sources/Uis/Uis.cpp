#include "Uis.hpp"

namespace acid
{
Uis::Uis() : m_container(nullptr, UiBound::Screen)
{
	for(auto button : EnumIterator<MouseButton>())
		{
			m_selectors.emplace(button, SelectorMouse());
		}
}

void Uis::Update()
{
	for(auto& [button, selector] : m_selectors)
		{
			bool isDown = Mouse::Get()->GetButton(button) != InputAction::Release;
			selector.m_wasDown = !selector.m_isDown && isDown;
			selector.m_isDown = isDown;
		}

	m_objects.clear();
	m_container.Update(m_objects);
}

void Uis::CancelWasEvent(const MouseButton& button)
{
	m_selectors[button].m_wasDown = false;
}

bool Uis::IsDown(const MouseButton& button)
{
	return m_selectors[button].m_isDown;
}

bool Uis::WasDown(const MouseButton& button)
{
	return m_selectors[button].m_wasDown;
}
}
