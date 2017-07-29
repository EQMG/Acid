#include "managerrender.h"

managerrender::managerrender()
{
	m_infinity = vector4(0.0f, 1.0f, 0.0f, +INFINITY);

	m_rendererEntities = new rendererentities();
	m_rendererGuis = new rendererguis();
	m_rendererFonts = new rendererfonts();

	//	m_fboRenderer = fbo::newFBO()->fitToScreen(1.0f)->attachments(1)->withAlphaChannel(false)->depthBuffer(TEXTURE)->create();
	//	m_filterFxaa = new filterfxaa(8.0f);
	//	m_filterGrain = new filtergrain(2.3f);
	//	m_filterCrt = new filtercrt(new colour(0.5f, 1.0f, 0.5f), 0.1f, 0.1f, 1000.0f, 0.1f);
	//	m_filterDefault = new filterdefault();

	//	m_pipelinePaused = new pipelinepaused();
}

managerrender::~managerrender()
{
	delete m_rendererEntities;
	delete m_rendererGuis;
	delete m_rendererFonts;

	delete m_fboRenderer;
	delete m_filterFxaa;
	delete m_filterGrain;
	delete m_filterCrt;
	delete m_filterDefault;

	//	delete m_pipelinePaused;
}

void managerrender::render()
{
	// Binds the render FBO.
	//	m_fboRenderer->bindFrameBuffer();

	// Scene rendering.
	icamera *camera = camera::get()->getCamera();
	renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 1.0f);
	m_rendererEntities->render(m_infinity, *camera);

	// Unbinds the render FBO.
	//	m_fboRenderer->unbindFrameBuffer();

	// Renders the post pipeline.
	//	fbo *output = m_fboRenderer;

	//m_filterFxaa->applyFilter(1, output->getColourTexture(0));
	//output = m_filterFxaa->getFbo();

	//	output->bindFrameBuffer();
	m_rendererGuis->render(m_infinity, *camera);
	m_rendererFonts->render(m_infinity, *camera);
	//	output->unbindFrameBuffer();

	//	m_filterCrt->applyFilter(1, output->getColourTexture(0));
	//	output = m_filterCrt->getFbo();

	//m_filterGrain->applyFilter(1, output->getColourTexture(0));
	//output = m_filterGrain->getFbo();

	// Displays the image to the screen.
	//	m_filterDefault->applyFilter(1, output->getColourTexture(0));
	//	output->blitToScreen();
}
