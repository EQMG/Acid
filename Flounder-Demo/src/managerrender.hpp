#pragma once

#include <flounder.hpp>
#include "particles/rendererparticles.hpp"
#include "skyboxes/rendererskyboxes.hpp"

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
	void renderWater();

	void renderShadows();

	void renderScene(const vector4 &clipPlane, const bool &waterPass);

	void renderPost();

	void renderGuis();
};
