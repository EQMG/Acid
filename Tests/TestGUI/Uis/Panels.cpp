#include "Panels.hpp"

namespace test
{
Panels::Panels(UiObject *parent) :
	UiObject(parent, UiTransform(Window::Get()->GetSize())),
	m_background(this, UiTransform(Window::Get()->GetSize()), Image2d::Create("Guis/White.png")),
	m_inventory(this),
	m_guiA(this, UiTransform(Vector2i(456, 367), Vector2i(64, 64), UiAnchor::LeftTop), Image2d::Create("Guis/White.png")),
	m_guiB(this, UiTransform(Vector2i(456, 367), Vector2i(-64, 64), UiAnchor::RightTop), Image2d::Create("Guis/White.png")),
	m_guiC(&m_guiB, UiTransform(Vector2i(256, 128), Vector2i(0, -72), UiAnchor::CentreBottom), Image2d::Create("Guis/White.png"))
{
	m_guiA.SetColourDriver(new DriverConstant<Colour>(Colour::Red));
	m_guiB.SetColourDriver(new DriverConstant<Colour>(Colour::Blue));
	m_guiC.SetColourDriver(new DriverConstant<Colour>(Colour::Yellow));
	m_guiB.OnSelected().Add([this](bool selected)
	{
		if (selected)
		{
			m_guiB.SetColourDriver(new DriverConstant<Colour>(Colour::Green));
		}
		else
		{
			m_guiB.SetColourDriver(new DriverConstant<Colour>(Colour::Blue));
		}
	});
}

void Panels::UpdateObject()
{
	GetTransform().SetSize(Window::Get()->GetSize());
	m_background.GetTransform().SetSize(Window::Get()->GetSize());
}
}
