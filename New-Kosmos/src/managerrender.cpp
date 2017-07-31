#include "managerrender.h"

managerrender::managerrender()
{
	m_infinity = vector4(0.0f, 1.0f, 0.0f, +INFINITY);

	m_rendererSkyboxes = new rendererskyboxes();
	m_rendererWaters = new rendererwaters();
	m_rendererParticles = new rendererparticles();
	m_rendererGuis = new rendererguis();
	m_rendererFonts = new rendererfonts();

	m_fboRenderer = fbo::newFBO()->fitToScreen(1.0f)->attachments(3)->withAlphaChannel(false)->depthBuffer(TEXTURE)->create();
	m_rendererDeferred = new rendererdeferred();
	m_filterFxaa = new filterfxaa(8.0f);
	m_filterGrain = new filtergrain(2.3f);
	m_filterCrt = new filtercrt(new colour(0.5f, 1.0f, 0.5f), 0.1f, 0.1f, 1000.0f, 0.1f);

	//	m_pipelinePaused = new pipelinepaused();
}

managerrender::~managerrender()
{
	delete m_rendererSkyboxes;
	delete m_rendererWaters;
	delete m_rendererParticles;
	delete m_rendererGuis;
	delete m_rendererFonts;

	delete m_fboRenderer;
	delete m_rendererDeferred;
	delete m_filterFxaa;
	delete m_filterGrain;
	delete m_filterCrt;

	//	delete m_pipelinePaused;
}

void managerrender::render()
{
	// Water rendering.
	renderWater(camera::get()->getCamera());

	// Shadow rendering.
	renderShadows(camera::get()->getCamera());

	// Binds the render FBO.
	m_fboRenderer->bindFrameBuffer();

	// Scene rendering
	renderScene(camera::get()->getCamera(), m_infinity, false);

	// Unbinds the render FBO.
	m_fboRenderer->unbindFrameBuffer();

	// Post rendering.
	renderPost(camera::get()->getCamera());
}

void managerrender::renderWater(icamera *camera)
{
	if (waters::get()->getEnableReflections() && waters::get()->getColourIntensity() != 1.0f && waters::get()->getWater() != NULL)
	{
		camera->reflect(waters::get()->getWater()->getPosition()->m_y);

		if (waters::get()->getReflectionShadows())
		{
			renderShadows(camera);
		}

		glEnable(GL_CLIP_DISTANCE0);
		{
			m_rendererWaters->getFboReflection()->bindFrameBuffer();
			renderScene(camera, vector4(0.0f, 1.0f, 0.0f, -waters::get()->getWater()->getPosition()->m_y), true);
			m_rendererWaters->getFboReflection()->unbindFrameBuffer();
		}
		glDisable(GL_CLIP_DISTANCE0);

		m_rendererWaters->getRendererDeferred()->apply(4, 
			m_rendererWaters->getFboReflection()->getColourTexture(0), 
			m_rendererWaters->getFboReflection()->getColourTexture(1), 
			m_rendererWaters->getFboReflection()->getColourTexture(2),
		 	m_rendererWaters->getFboReflection()->getDepthTexture()
			// shadowRenderer->getShadowMap()
		); // Shadow Map -  Depth -  Extras -  Normals -  Colours

		camera->reflect(waters::get()->getWater()->getPosition()->m_y);
	}
}

void managerrender::renderShadows(icamera *camera)
{
}

void managerrender::renderScene(icamera *camera, const vector4 &clipPlane, const bool &waterPass)
{
	renderer::get()->prepareNewRenderParse(0.0f, 0.0f, 0.0f, 0.0f);
	m_rendererSkyboxes->render(clipPlane, *camera);

	if (!waterPass)
	{
		m_rendererWaters->render(clipPlane, *camera);
	}

	m_rendererParticles->render(clipPlane, *camera);
}

void managerrender::renderPost(icamera *camera)
{
	// Renders the post pipeline.
	fbo *output = m_fboRenderer;

	m_rendererDeferred->apply(4,
		output->getColourTexture(0),
		output->getColourTexture(1),
		output->getColourTexture(2),
		output->getDepthTexture()
		//shadowRenderer->getShadowMap()
	);
	output = m_rendererDeferred->getFbo();

	m_filterFxaa->applyFilter(1, output->getColourTexture(0));
	output = m_filterFxaa->getFbo();

	output->bindFrameBuffer();
	renderGuis(camera);
	output->unbindFrameBuffer();

	//  m_filterCrt->applyFilter(1, output->getColourTexture(0));
	//  output = m_filterCrt->getFbo();

	//  m_filterGrain->applyFilter(1, output->getColourTexture(0));
	//  output = m_filterGrain->getFbo();

	// Displays the image to the screen.
	output->blitToScreen();
}

void managerrender::renderGuis(icamera *camera)
{
	m_rendererGuis->render(m_infinity, *camera);
	m_rendererFonts->render(m_infinity, *camera);
}
