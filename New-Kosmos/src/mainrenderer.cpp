#include "mainrenderer.h"

mainrenderer::mainrenderer()
{
	m_infinity = vector4(0.0f, 1.0f, 0.0f, +INFINITY);
	m_skybox = new rendererskybox();

	m_fboRenderer = fbo::newFBO()->fitToScreen(1.0f)->attachments(3)->withAlphaChannel(true)->depthBuffer(TEXTURE)->create();
	m_deferred = new deferredrenderer();
	m_filterFxaa = new filterfxaa(8.0f);
	m_filterGrain = new filtergrain(2.3f);
	m_filterCrt = new filtercrt(new colour(0.5f, 1.0f, 0.5f), 0.175f, 0.175f, 1024.0f, 0.09f);
}

mainrenderer::~mainrenderer()
{
	delete m_skybox;

	delete m_fboRenderer;
	delete m_deferred;
	delete m_filterFxaa;
	delete m_filterGrain;
	delete m_filterCrt;
}

void mainrenderer::render()
{
	// Binds the render FBO.
	m_fboRenderer->bindFrameBuffer();

	// Scene rendering.
	renderer::get()->prepareNewRenderParse(mouse::get()->getPositionX(), mouse::get()->getPositionY(), 0.0f, 0.0f);
	m_skybox->render(m_infinity, *camera::get()->getCamera());

	// Unbinds the render FBO.
	m_fboRenderer->unbindFrameBuffer();

	// Renders the post pipeline.
	fbo *output = m_fboRenderer;

	m_deferred->apply(4,
	                  output->getColourTexture(0),
	                  output->getColourTexture(1),
	                  output->getColourTexture(2),
	                  output->getDepthTexture()
	                  //shadowRenderer->getShadowMap()
	);
	output = m_deferred->getFbo();

	m_filterFxaa->applyFilter(1, output->getColourTexture(0));
	output = m_filterFxaa->getFbo();

	m_filterGrain->applyFilter(1, output->getColourTexture(0));
	output = m_filterGrain->getFbo();

	m_filterCrt->applyFilter(1, output->getColourTexture(0));
	output = m_filterCrt->getFbo();

	// Displays the image to the screen.
	output->blitToScreen();
}
