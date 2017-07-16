#include "maininstance.h"

namespace flounder {
	maininstance::maininstance()
	{
		m_buttonFullscreen = new buttonkeyboard(1, GLFW_KEY_F11);
		m_buttonWireframe = new buttonkeyboard(1, GLFW_KEY_P);
		m_buttonExit = new buttonkeyboard(1, GLFW_KEY_DELETE);
	}

	maininstance::~maininstance()
	{
		delete m_buttonFullscreen;
		delete m_buttonWireframe;
		delete m_buttonExit;
	}

	void maininstance::update()
	{
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
	}
}
