#include "Panels.hpp"

namespace test
{
int32_t GetNextY()
{
	static uint32_t i = 0;
	return i++ * (UiInputButton::Size.m_y + 8);
}

Vector2i SIZE3 = (UiInputButton::Size - Vector2i(5, 0)) * Vector2f(1.0f/3.0f, 1.0f);

Panels::Panels(UiObject *parent) :
	UiObject(parent, UiTransform(Window::Get()->GetSize())),
	m_background(this, UiTransform(Window::Get()->GetSize()), Image2d::Create("Guis/White.png")),
	m_inventory(this),
	m_gui0(this, UiTransform(Vector2i(456, 500), UiAnchor::LeftTop, Vector2i(64, 64)), UiInputButton::BackgroundColour, UiPanel::Resize::Right, ScrollBar::None),
	m_boolean0(&m_gui0.GetContent(), "Boolean", false, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_button0(&m_gui0.GetContent(), "Button", UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_dropdown0(&m_gui0.GetContent(), "Dropdown", 0, { "Option A", "Option B", "Option C" }, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_grabber0(&m_gui0.GetContent(), "Grabber Joystick", 0, 0, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_grabber1(&m_gui0.GetContent(), "Grabber Keyboard", Key::A, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_grabber2(&m_gui0.GetContent(), "Grabber Mouse", MouseButton::Left, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_radio0a(&m_gui0.GetContent(), "Radio A", UiInputRadio::Type::Filled, false, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_radio0b(&m_gui0.GetContent(), "Radio B", UiInputRadio::Type::Filled, true, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_radio0c(&m_gui0.GetContent(), "Radio C", UiInputRadio::Type::Filled, false, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_radioManager0(UiInputRadio::Type::Filled, false, { &m_radio0a, &m_radio0b, &m_radio0c }),
	m_slider0(&m_gui0.GetContent(), "Slider", 50.0f, -100.0f, 100.0f, 1, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_text0(&m_gui0.GetContent(), "Text", "ABC123", 16, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_textX(&m_gui0.GetContent(), "X", "0.0", 16, UiTransform(SIZE3, UiAnchor::LeftTop, Vector2i(0, GetNextY()))),
	m_textY(&m_gui0.GetContent(), "Y", "1.0", 16, UiTransform(SIZE3, UiAnchor::LeftTop, Vector2i((SIZE3.m_x) + 3.5, m_textX.GetTransform().GetPosition().m_y))),
	m_textZ(&m_gui0.GetContent(), "Z", "1.0", 16, UiTransform(SIZE3, UiAnchor::LeftTop, Vector2i((2 * SIZE3.m_x) + 7, m_textX.GetTransform().GetPosition().m_y))),
	m_gui1(this, UiTransform(Vector2i(456, 367), UiAnchor::RightTop, Vector2i(-64, 64)), Image2d::Create("Guis/White.png")),
	m_gui2(&m_gui1, UiTransform(Vector2i(256, 128), UiAnchor::CentreBottom, Vector2i(0, -72)), Image2d::Create("Guis/White.png")),
	m_text1(this, UiTransform(Vector2i(256, 256), UiAnchor::LeftBottom, Vector2i(50, -75)), 72, "|ABC abc 0123_*&.", FontType::Create("Fonts/ProximaNova", "Regular"))
{
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
	m_text1.SetEnabled(false);
}

void Panels::UpdateObject()
{
	GetTransform().SetSize(Window::Get()->GetSize());
	m_background.GetTransform().SetSize(Window::Get()->GetSize());
}
}
