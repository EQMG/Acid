#pragma once

#include <flounder.h>

using namespace flounder;

class managerrender :
	public imanagerrender
{
private:
	vector4 m_infinity;

	rendererskyboxes *m_rendererSkyboxes;
	rendererparticles *m_rendererParticles;
	rendererguis *m_rendererGuis;
	rendererfonts *m_rendererFonts;

	fbo *m_fboRenderer;
	deferredrenderer *m_deferred;
	filterfxaa *m_filterFxaa;
	filtergrain *m_filterGrain;
	filterlensflare *m_filterLensflare;
	filtertiltshift *m_filterTiltshift;
	filtercrt *m_filterCrt;

	pipelinebloom *m_pipelineBloom;
	pipelinepaused *m_pipelinePaused;
public:
	managerrender();

	~managerrender();

	void render() override;
};
