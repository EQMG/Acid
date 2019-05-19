#include "Panels.hpp"

namespace test
{
Panels::Panels(UiObject *parent) :
	UiObject(parent, UiTransform::Screen),
	m_hierarchy(this),
	m_inspector(this)
{
}

void Panels::UpdateObject()
{
}
}
