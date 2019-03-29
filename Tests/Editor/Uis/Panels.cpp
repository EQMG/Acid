#include "Panels.hpp"

namespace test
{
	Panels::Panels(UiObject *parent) :
		UiObject(parent, UiBound::Screen),
		m_hierarchy(Hierarchy(this)),
		m_inspector(Inspector(this))
	{
	}

	void Panels::UpdateObject()
	{
	}
}
