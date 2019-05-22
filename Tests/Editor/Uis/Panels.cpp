#include "Panels.hpp"

namespace test
{
Panels::Panels(UiObject *parent) :
	UiObject(parent, UiTransform(Window::Get()->GetSize())),
	m_hierarchy(this),
	m_inspector(this)
{
}

void Panels::UpdateObject()
{
	GetTransform().SetSize(Window::Get()->GetSize());
}
}
