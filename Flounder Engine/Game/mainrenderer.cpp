#include "mainrenderer.h"

namespace flounder {
	mainrenderer::mainrenderer()
	{
		m_infinity = new vector4(0.0f, 1.0f, 0.0f, INFINITY);
		m_skybox = new rendererskybox();

		m_fboRenderer = new fbo((new fbobuilder())->fitToScreen(1.0f)->attachments(3)->withAlphaChannel(false)->depthBuffer(TEXTURE));
	}

	mainrenderer::~mainrenderer()
	{
		delete m_infinity;
		delete m_skybox;
		delete m_fboRenderer;
	}

	void mainrenderer::render()
	{
		// Binds the render FBO.
		m_fboRenderer->bindFrameBuffer();

		// Scene rendering.
		renderer::get()->prepareNewRenderParse(0.1f, 0.0f, 0.0f);
		m_skybox->render(m_infinity, camera::get()->getCamera());

		// Unbinds the render FBO.
		m_fboRenderer->unbindFrameBuffer();

		// Renders the post pipeline.
		fbo *output = m_fboRenderer;

		// Displays the image to the screen.
		output->blitToScreen();
	}
}
