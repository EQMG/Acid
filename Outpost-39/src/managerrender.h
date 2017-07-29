#pragma once

#include <flounder.h>

using namespace flounder;

class managerrender :
	public imanagerrender
{
private:
	vector4 m_infinity;

	rendererguis *m_rendererGuis;
	rendererfonts *m_rendererFonts;

	fbo *m_fboRenderer;
	filterfxaa *m_filterFxaa;
	filtergrain *m_filterGrain;
	filtercrt *m_filterCrt;
	filterdefault *m_filterDefault;

	pipelinepaused *m_pipelinePaused;
public:
	managerrender();

	~managerrender();

	void render() override;
};
