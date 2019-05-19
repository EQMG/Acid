#include "Panels.hpp"

namespace test
{
Panels::Panels(UiObject *parent) :
	UiObject(parent, UiTransform::Screen),
	m_background(this, UiTransform::Screen, Image2d::Create("Guis/White.png")),
	m_hierarchy(this),
	m_inspector(this)
{
}

void Panels::UpdateObject()
{
}
}
