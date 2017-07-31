#include "managerrender.h"

managerrender::managerrender()
{
	m_infinity = vector4(0.0f, 1.0f, 0.0f, +INFINITY);

	m_rendererSkyboxes = new rendererskyboxes();
	m_rendererEntities = new rendererentities();
	m_rendererParticles = new rendererparticles();
	m_rendererGuis = new rendererguis();
	m_rendererFonts = new rendererfonts();

	m_fboRenderer = fbo::newFBO()->fitToScreen(1.0f)->attachments(3)->withAlphaChannel(false)->depthBuffer(TEXTURE)->create();
	m_deferred = new deferredrenderer();
	m_filterFxaa = new filterfxaa(8.0f);
	m_filterGrain = new filtergrain(2.3f);
	m_filterCrt = new filtercrt(new colour(0.5f, 1.0f, 0.5f), 0.1f, 0.1f, 1000.0f, 0.1f);

	//	m_pipelinePaused = new pipelinepaused();
}

managerrender::~managerrender()
{
	delete m_rendererSkyboxes;
	delete m_rendererEntities;
	delete m_rendererParticles;
	delete m_rendererGuis;
	delete m_rendererFonts;

	delete m_fboRenderer;
	delete m_deferred;
	delete m_filterFxaa;
	delete m_filterGrain;
	delete m_filterCrt;

	//	delete m_pipelinePaused;
}

void managerrender::render()
{
	// Binds the render FBO.
	m_fboRenderer->bindFrameBuffer();

	// Scene rendering.
	icamera *camera = camera::get()->getCamera();
	renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 0.0f);
	m_rendererSkyboxes->render(m_infinity, *camera);
	m_rendererEntities->render(m_infinity, *camera);
	m_rendererParticles->render(m_infinity, *camera);

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

	//  m_filterFxaa->applyFilter(1, output->getColourTexture(0));
	//  output = m_filterFxaa->getFbo();

	output->bindFrameBuffer();
	m_rendererGuis->render(m_infinity, *camera);
	m_rendererFonts->render(m_infinity, *camera);
	output->unbindFrameBuffer();

	//  m_filterCrt->applyFilter(1, output->getColourTexture(0));
	//  output = m_filterCrt->getFbo();

	//  m_filterGrain->applyFilter(1, output->getColourTexture(0));
	//  output = m_filterGrain->getFbo();

	// Displays the image to the screen.
	output->blitToScreen();
}
