#include "mainrenderer.h"

namespace flounder {
	mainrenderer::mainrenderer()
	{
		m_infinity = new vector4(0.0f, 1.0f, 0.0f, INFINITY);
		m_skybox = new rendererskybox();

		m_fboTest = new fbo(new fbobuilder());
	}

	mainrenderer::~mainrenderer()
	{
		delete m_infinity;
		delete m_skybox;
		delete m_fboTest;
	}

	void mainrenderer::render()
	{
		renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f);
		m_skybox->render(m_infinity, camera::get()->getCamera());
	}
}
