#include "Panels.hpp"

#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
	Panels::Panels(UiObject *parent) :
		UiObject(parent, UiBound::Screen),
		m_hierarchy(std::make_unique<Hierarchy>(this)),
		m_inspector(std::make_unique<Inspector>(this))
	{
	}

	void Panels::UpdateObject()
	{
	}
}
