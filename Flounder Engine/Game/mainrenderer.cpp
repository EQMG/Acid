#include "mainrenderer.h"

namespace flounder {
	mainrenderer::mainrenderer()
	{
	}

	mainrenderer::~mainrenderer()
	{
	}

	void mainrenderer::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor((GLclampf) 0.0, (GLclampf) 0.0, (GLclampf)mouse::get()->getPositionY(), (GLclampf) 1.0);
	}
}
