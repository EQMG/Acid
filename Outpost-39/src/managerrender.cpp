#include "managerrender.h"

managerrender::managerrender()
{
	m_infinity = vector4(0.0f, 1.0f, 0.0f, +INFINITY);

	m_rendererGuis = new rendererguis();
	m_rendererFonts = new rendererfonts();

//	m_fboRenderer = fbo::newFBO()->fitToScreen(1.0f)->attachments(3)->withAlphaChannel(false)->depthBuffer(TEXTURE)->create();
//	m_filterFxaa = new filterfxaa(8.0f);
//	m_filterGrain = new filtergrain(2.3f);
//	m_filterCrt = new filtercrt(new colour(0.5f, 1.0f, 0.5f), 0.175f, 0.175f, 1024.0f, 0.09f);

//	m_pipelinePaused = new pipelinepaused();
}

managerrender::~managerrender()
{
	delete m_rendererGuis;
	delete m_rendererFonts;

	delete m_fboRenderer;
	delete m_filterFxaa;
	delete m_filterGrain;
	delete m_filterCrt;

//	delete m_pipelinePaused;
}

void managerrender::render()
{
	// Binds the render FBO.
//	m_fboRenderer->bindFrameBuffer();

	// Scene rendering.
	icamera *camera = camera::get()->getCamera();
	renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 1.0f);

	// Unbinds the render FBO.
//	m_fboRenderer->unbindFrameBuffer();

	// Renders the post pipeline.
//	fbo *output = m_fboRenderer;

	//m_filterFxaa->applyFilter(1, output->getColourTexture(0));
	//output = m_filterFxaa->getFbo();

	//m_filterGrain->applyFilter(1, output->getColourTexture(0));
	//output = m_filterGrain->getFbo();

	//m_filterCrt->applyFilter(1, output->getColourTexture(0));
	//output = m_filterCrt->getFbo();

//	output->bindFrameBuffer();
	m_rendererGuis->render(m_infinity, *camera);
	m_rendererFonts->render(m_infinity, *camera);
//	output->unbindFrameBuffer();

	// Displays the image to the screen.
//	output->blitToScreen();
}
