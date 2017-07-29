#include "instance.h"

instance::instance()
{
	m_buttonFullscreen = new buttonkeyboard(1, GLFW_KEY_F11);
	m_buttonWireframe = new buttonkeyboard(1, GLFW_KEY_P);
	m_buttonExit = new buttonkeyboard(1, GLFW_KEY_DELETE);
}

instance::~instance()
{
	delete m_buttonFullscreen;
	delete m_buttonWireframe;
	delete m_buttonExit;
}

void instance::update()
{
#ifndef FLOUNDER_PLATFORM_WEB
	if (m_buttonFullscreen->wasDown())
	{
		display::get()->setFullscreen(!display::get()->isFullscreen());
	}

	if (m_buttonWireframe->wasDown())
	{
		renderer::get()->goWireframe(!renderer::get()->isInWireframe());
	}

	if (m_buttonExit->wasDown())
	{
		framework::get()->requestClose(false);
	}
#endif
}
