#include "managerrender.hpp"

managerrender::managerrender() :
	m_infinity(vector4(0.0f, 1.0f, 0.0f, +INFINITY))
	//m_rendererShadows(new renderershadows()),
	//m_rendererSkyboxes(new rendererskyboxes()),
	//m_rendererTerrains(new rendererterrains()),
	//m_rendererWaters(new rendererwaters()),
	//m_rendererParticles(new rendererparticles()),
	//m_rendererGuis(new rendererguis()),
	//m_rendererFonts(new rendererfonts()),
	//m_fboRenderer(new fbo(true, 1.0f, TEXTURE, false)),
	//m_rendererDeferred(new rendererdeferred()),
	//m_filterFxaa(new filterfxaa()),
	//m_filterLensFlare(new filterlensflare())
{
}

managerrender::~managerrender()
{
	//delete m_rendererShadows;
	//delete m_rendererSkyboxes;
	//delete m_rendererTerrains;
	//delete m_rendererWaters;
	//delete m_rendererParticles;
	//delete m_rendererGuis;
	//delete m_rendererFonts;

	//delete m_fboRenderer;
	//delete m_rendererDeferred;
	//delete m_filterFxaa;
}

void managerrender::render()
{
#if 0
	// Water rendering.
	renderWater();

	// Shadow rendering.
	renderShadows();

	// Binds the render FBO.
	m_fboRenderer->bindFrameBuffer();

	// Scene rendering
	renderScene(m_infinity, false);

	// Unbinds the render FBO.
	m_fboRenderer->unbindFrameBuffer();

	// Post rendering.
	renderPost();
	m_rendererShadows->render(m_infinity);
	m_filterFxaa->applyFilter(1, m_rendererShadows->getFbo()->getDepthTexture());
	m_filterFxaa->getFbo()->blitToScreen();
#endif
}

void managerrender::renderWater()
{
#if 0
	if (waters::get()->getEnableReflections() && waters::get()->getColourIntensity() != 1.0f && waters::get()->getWater() != nullptr)
	{
		camera->reflect(waters::get()->getWater()->getPosition()->m_y);

		if (waters::get()->getReflectionShadows())
		{
			renderShadows();
		}

		glEnable(GL_CLIP_DISTANCE0);
		{
			m_rendererWaters->getFboReflection()->bindFrameBuffer();
			renderScene(camera, vector4(0.0f, 1.0f, 0.0f, -waters::get()->getWater()->getPosition()->m_y), true);
			m_rendererWaters->getFboReflection()->unbindFrameBuffer();
		}
		glDisable(GL_CLIP_DISTANCE0);

		m_rendererWaters->getRendererDeferred()->apply(5, 
			m_rendererWaters->getFboReflection()->getColourTexture(0), 
			m_rendererWaters->getFboReflection()->getColourTexture(1), 
			m_rendererWaters->getFboReflection()->getColourTexture(2),
		 	m_rendererWaters->getFboReflection()->getDepthTexture(),
			m_rendererShadows->getFbo()->getDepthTexture()
		); // Shadow Map -  Depth -  Extras -  Normals -  Colours

		camera->reflect(waters::get()->getWater()->getPosition()->m_y);
	}
#endif
}

void managerrender::renderShadows()
{
#if 0
	m_rendererShadows->render(m_infinity, *camera);
#endif
}

void managerrender::renderScene(const vector4 &clipPlane, const bool &waterPass)
{
#if 0
	renderer::get()->prepareNewRenderParse(1.0f, 0.0f, 0.0f, 1.0f);
	m_rendererSkyboxes->render(clipPlane, *camera);
	m_rendererTerrains->render(clipPlane, *camera);

	if (!waterPass)
	{
		m_rendererWaters->render(clipPlane, *camera);
	}

	m_rendererParticles->render(clipPlane, *camera);
#endif
}

void managerrender::renderPost()
{
#if 0
	// Renders the post pipeline.
	fbo *output = m_fboRenderer;

	m_rendererDeferred->apply(5,
		output->getColourTexture(0),
		output->getColourTexture(1),
		output->getColourTexture(2),
		output->getDepthTexture(),
		m_rendererShadows->getFbo()->getDepthTexture()
	);
	output = m_rendererDeferred->getFbo();

	m_filterFxaa->applyFilter(1, output->getColourTexture(0));
	output = m_filterFxaa->getFbo();

	output->bindFrameBuffer();
	renderGuis(camera);
	output->unbindFrameBuffer();

	m_filterLensFlare->setSunPosition(*worlds::get()->getSunPosition());
	m_filterLensFlare->setSunHeight(worlds::get()->getSunHeight());
	m_filterLensFlare->applyFilter(1, output->getColourTexture(0));
	output = m_filterLensFlare->getFbo();

	// Displays the image to the screen.
	output->blitToScreen();
#endif
}

void managerrender::renderGuis()
{
#if 0
	m_rendererGuis->render(m_infinity, *camera);
	m_rendererFonts->render(m_infinity, *camera);
#endif
}
