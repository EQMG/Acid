#pragma once

#include <Flounder.hpp>
#include "particles/rendererparticles.hpp"
#include "skyboxes/rendererskyboxes.hpp"

using namespace Flounder;

class ManagerRender :
	public IManagerRender
{
private:
	Vector4 m_infinity;

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
	ManagerRender();

	~ManagerRender();

	void Render() override;
private:
	void renderWater();

	void renderShadows();

	void renderScene(const Vector4 &clipPlane, const bool &waterPass);

	void renderPost();

	void renderGuis();
};
