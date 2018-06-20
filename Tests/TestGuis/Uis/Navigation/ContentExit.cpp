#include "ContentExit.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.4375f), "Centre", true, false, Vector2(0.8f, 0.7f))),
		m_background(new Gui(this, *GetRectangle(), Texture::Resource("Guis/White.png"), 0)),
		m_button1(new UiInputButton(m_background, Vector2(0.5f, 0.4375f), "Test Button", JUSTIFY_CENTRE))
	{
	}

	ContentExit::~ContentExit()
	{
		delete m_background;
		delete m_button1;
	}

	void ContentExit::UpdateObject()
	{
	}
}
