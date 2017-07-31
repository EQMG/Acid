#pragma once

#include <flounder.h>
#include "particles/rendererparticles.h"
#include "skyboxes/rendererskyboxes.h"

using namespace flounder;

class managerrender :
	public imanagerrender
{
private:
	vector4 m_infinity;

	rendererskyboxes *m_rendererSkyboxes;
	rendererentities *m_rendererEntities;
	rendererparticles *m_rendererParticles;
	rendererguis *m_rendererGuis;
	rendererfonts *m_rendererFonts;

	fbo *m_fboRenderer;
	deferredrenderer *m_deferred;
	filterfxaa *m_filterFxaa;
	filtergrain *m_filterGrain;
	filtercrt *m_filterCrt;

	pipelinepaused *m_pipelinePaused;
public:
	managerrender();

	~managerrender();

	void render() override;
};
