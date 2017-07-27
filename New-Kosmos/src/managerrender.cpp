#include "managerrender.h"

managerrender::managerrender()
{
	m_infinity = vector4(0.0f, 1.0f, 0.0f, +INFINITY);

	m_rendererSkybox = new rendererskybox();
	m_rendererGuis = new rendererguis();
	m_rendererFonts = new rendererfonts();

	m_fboRenderer = fbo::newFBO()->fitToScreen(1.0f)->attachments(3)->withAlphaChannel(false)->depthBuffer(TEXTURE)->create();
	m_deferred = new deferredrenderer();
	m_filterFxaa = new filterfxaa(8.0f);
	m_filterGrain = new filtergrain(2.3f);
	m_filterLensflare = new filterlensflare();
	m_filterTiltshift = new filtertiltshift();
	m_filterCrt = new filtercrt(new colour(0.5f, 1.0f, 0.5f), 0.175f, 0.175f, 1024.0f, 0.09f);

	m_pipelineBloom = new pipelinebloom();
	m_pipelinePaused = new pipelinepaused();
}

managerrender::~managerrender()
{
	delete m_rendererSkybox;
	delete m_rendererGuis;
	delete m_rendererFonts;

	delete m_fboRenderer;
	delete m_deferred;
	delete m_filterFxaa;
	delete m_filterGrain;
	delete m_filterLensflare;
	delete m_filterTiltshift;
	delete m_filterCrt;

	delete m_pipelineBloom;
	delete m_pipelinePaused;
}

void managerrender::render()
{
	// Binds the render FBO.
	m_fboRenderer->bindFrameBuffer();

	// Scene rendering.
	icamera *camera = camera::get()->getCamera();
	renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 0.0f);
	m_rendererSkybox->render(m_infinity, *camera);

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

	//m_filterFxaa->applyFilter(1, output->getColourTexture(0));
	//output = m_filterFxaa->getFbo();

	//m_filterGrain->applyFilter(1, output->getColourTexture(0));
	//output = m_filterGrain->getFbo();

	//m_filterLensflare->setSunPosition(vector3(100.0f, 10.0f, 0.0f));
	//m_filterLensflare->setSunHeight(0.0f);
	//m_filterLensflare->applyFilter(1, output->getColourTexture(0));
	//output = m_filterLensflare->getFbo();

	m_filterTiltshift->applyFilter(1, output->getColourTexture(0));
	output = m_filterTiltshift->getFbo();

	//m_filterCrt->applyFilter(1, output->getColourTexture(0));
	//output = m_filterCrt->getFbo();

	//m_pipelinePaused->setBlurFactor(0.1f);
	//m_pipelinePaused->renderPipelineV(1, output->getColourTexture(0));
	//output = m_pipelinePaused->getOutput();

	output->bindFrameBuffer();
	m_rendererGuis->render(m_infinity, *camera);
	m_rendererFonts->render(m_infinity, *camera);
	output->unbindFrameBuffer();

	// Displays the image to the screen.
	output->blitToScreen();
}
