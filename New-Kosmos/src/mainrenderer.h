#pragma once

#include <flounder.h>

using namespace flounder;

class mainrenderer :
	public irenderermaster
{
private:
	vector4 m_infinity;

	rendererskybox *m_skybox;

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
	mainrenderer();

	~mainrenderer();

	void render() override;
};
