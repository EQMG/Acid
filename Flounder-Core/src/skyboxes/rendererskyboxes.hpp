#pragma once

#include "../renderer/irenderer.hpp"

#include "skyboxes.hpp"

namespace Flounder
{
	class rendererskyboxes :
		public irenderer
	{
	private:
		shader *m_shader;
	public:
		rendererskyboxes();

		~rendererskyboxes();

		void render(const vector4 &clipPlane, const ICamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const ICamera &camera);

		void renderSkybox(skybox *object);

		void endRendering();
	};
}
