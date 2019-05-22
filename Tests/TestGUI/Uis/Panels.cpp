#include "Panels.hpp"

namespace test
{
int32_t GetNextY()
{
	static uint32_t i = 0;
	return 8 + (i++ * 37);
}

Panels::Panels(UiObject *parent) :
	UiObject(parent, UiTransform(Window::Get()->GetSize())),
	m_background(this, UiTransform(Window::Get()->GetSize()), Image2d::Create("Guis/White.png")),
	m_inventory(this),
	m_gui0(this, UiTransform(Vector2i(456, 367), Vector2i(64, 64), UiAnchor::LeftTop), UiInputButton::BackgroundColour, UiPanel::Resize::Right, ScrollBar::None),
	m_boolean0(&m_gui0.GetContent(), "Boolean", false, UiTransform(UiInputButton::Size, Vector2i(8, GetNextY()), UiAnchor::LeftTop)),
	m_button0(&m_gui0.GetContent(), "Button", UiTransform(UiInputButton::Size, Vector2i(8, GetNextY()), UiAnchor::LeftTop)),
	m_grabber0(&m_gui0.GetContent(), "Grabber Joystick", 0, 0, UiTransform(UiInputButton::Size, Vector2i(8, GetNextY()), UiAnchor::LeftTop)),
	m_grabber1(&m_gui0.GetContent(), "Grabber Keyboard", Key::A, UiTransform(UiInputButton::Size, Vector2i(8, GetNextY()), UiAnchor::LeftTop)),
	m_grabber2(&m_gui0.GetContent(), "Grabber Mouse", MouseButton::Left, UiTransform(UiInputButton::Size, Vector2i(8, GetNextY()), UiAnchor::LeftTop)),
	//m_radio0a(&m_gui0.GetContent(), "Radio A", UiInputRadio::Type::Filled, false, UiTransform(UiInputButton::Size, Vector2i(8, GetNextY()), UiAnchor::LeftTop)),
	//m_radio0b(&m_gui0.GetContent(), "Radio B", UiInputRadio::Type::Filled, false, UiTransform(UiInputButton::Size, Vector2i(8, GetNextY()), UiAnchor::LeftTop)),
	//m_radioManager0(UiInputRadio::Type::X, false, { &m_radio0a, &m_radio0b }),
	m_slider0(&m_gui0.GetContent(), "Slider", 50.0f, -100.0f, 100.0f, 1, UiTransform(UiInputButton::Size, Vector2i(8, GetNextY()), UiAnchor::LeftTop)),
	m_text0(&m_gui0.GetContent(), "Text", "ABC123", 16, UiTransform(UiInputButton::Size, Vector2i(8, GetNextY()), UiAnchor::LeftTop)),
	m_gui1(this, UiTransform(Vector2i(456, 367), Vector2i(-64, 64), UiAnchor::RightTop), Image2d::Create("Guis/White.png")),
	m_gui2(&m_gui2, UiTransform(Vector2i(256, 128), Vector2i(0, -72), UiAnchor::CentreBottom), Image2d::Create("Guis/White.png"))
{
	//m_gui0.SetScaleDriver(new DriverSinwave<Vector2f>(Vector2f(0.9f), Vector2f(1.2f), 6s));

	m_gui1.SetColourDriver(new DriverConstant<Colour>(Colour::Blue));
	m_gui2.SetColourDriver(new DriverConstant<Colour>(Colour::Yellow));
	m_gui1.OnSelected().Add([this](bool selected)
	{
		if (selected)
		{
			m_gui1.SetColourDriver(new DriverConstant<Colour>(Colour::Green));
		}
		else
		{
			m_gui1.SetColourDriver(new DriverConstant<Colour>(Colour::Blue));
		}
	});
}

void Panels::UpdateObject()
{
	GetTransform().SetSize(Window::Get()->GetSize());
	m_background.GetTransform().SetSize(Window::Get()->GetSize());
}
}
