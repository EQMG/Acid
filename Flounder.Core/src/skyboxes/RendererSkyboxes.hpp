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

		void render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void prepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void renderSkybox(skybox *object);

		void endRendering();
	};
}
