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

	//renderershadows *m_rendererShadows;
	//rendererskyboxes *m_rendererSkyboxes;
	//rendererterrains *m_rendererTerrains;
	//rendererwaters *m_rendererWaters;
	//rendererparticles *m_rendererParticles;
	//rendererguis *m_rendererGuis;
	//rendererfonts *m_rendererFonts;

	//fbo *m_fboRenderer;
	//rendererdeferred *m_rendererDeferred;
	//filterfxaa *m_filterFxaa;
	//filterlensflare *m_filterLensFlare;
public:
	managerrender();

	~managerrender();

	void render() override;
private:
	void renderWater(icamera *camera);

	void renderShadows(icamera *camera);

	void renderScene(icamera *camera, const vector4 &clipPlane, const bool &waterPass);

	void renderPost(icamera *camera);

	void renderGuis(icamera *camera);
};
