#include "mainrenderer.h"

namespace flounder {
	mainrenderer::mainrenderer()
	{
		m_infinity = new vector4(0.0f, 1.0f, 0.0f, INFINITY);
		//m_skybox = new rendererskybox();

		m_fboRenderer = fbo::newFBO()->fitToScreen(1.0f)->attachments(3)->withAlphaChannel(false)->depthBuffer(TEXTURE)->create();

		m_driverR = new driversinwave(0.0f, 1.0f, 3.0f);
	}

	mainrenderer::~mainrenderer()
	{
		delete m_infinity;
		delete m_skybox;
		delete m_fboRenderer;
		delete m_driverR;
	}

	void mainrenderer::render()
	{
		// Binds the render FBO.
		m_fboRenderer->bindFrameBuffer();

		// Scene rendering.
		float r = m_driverR->update(framework::get()->getDeltaRender());
		renderer::get()->prepareNewRenderParse(r, 0.0f, 0.0f);
		//m_skybox->render(m_infinity, camera::get()->getCamera());

		// Unbinds the render FBO.
		m_fboRenderer->unbindFrameBuffer();

		// Renders the post pipeline.
		fbo *output = m_fboRenderer;

		// Displays the image to the screen.
		output->blitToScreen();
	}
}
