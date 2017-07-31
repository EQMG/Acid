#pragma once

#include "../renderer/irenderer.h"

#include "skyboxes.h"

namespace flounder
{
	class rendererskyboxes :
		public irenderer
	{
	private:
		shader *m_shader;
	public:
		rendererskyboxes();

		~rendererskyboxes();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void renderSkybox(skybox *object);

		void endRendering();
	};
}
