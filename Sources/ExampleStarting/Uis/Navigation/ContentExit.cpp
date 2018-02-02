#include "ContentExit.hpp"

#include <Uis/InputButton.hpp>

namespace Demo
{
	ContentExit::ContentExit(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.4375f), "Centre", true, false, Vector2(0.8f, 0.7f))),
		m_background(new Gui(this, *GetRectangle(), Texture::Resource("Resources/Guis/White.png"), 0))
	{
		InputButton *button1 = new InputButton(m_background, Vector2(0.5f, 0.4375f), "Test Button", JustifyCentre);
	}

	ContentExit::~ContentExit()
	{
		delete m_background;
	}

	void ContentExit::UpdateObject()
	{
	}
}
