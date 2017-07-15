#include "mainrenderer.h"

namespace flounder {
	mainrenderer::mainrenderer()
	{
		m_infinity = new vector4(0.0f, 1.0f, 0.0f, +INFINITY);
		m_skybox = new rendererskybox();

		m_fboRenderer = fbo::newFBO()->fitToScreen(1.0f)->attachments(3)->withAlphaChannel(false)->depthBuffer(TEXTURE)->create();
		m_filterNegative = new filternegative();
	}

	mainrenderer::~mainrenderer()
	{
		delete m_infinity;
		delete m_skybox;
		delete m_fboRenderer;
		delete m_filterNegative;
	}

	void mainrenderer::render()
	{
		// Binds the render FBO.
		m_fboRenderer->bindFrameBuffer();

		// Scene rendering.
		renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f);
		m_skybox->render(m_infinity, camera::get()->getCamera());

		// Unbinds the render FBO.
		m_fboRenderer->unbindFrameBuffer();

		// Renders the post pipeline.
		fbo *output = m_fboRenderer;

		m_filterNegative->applyFilter(1, output->getColourTexture(0));
		output = m_filterNegative->getFbo();

		// Displays the image to the screen.
		output->blitToScreen();
	}
}
