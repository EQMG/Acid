#pragma once

#include "../renderer/IRenderer.hpp"

#include "Skyboxes.hpp"

namespace Flounder
{
	class RendererSkyboxes :
		public IRenderer
	{
	private:
		Shader *m_shader;
	public:
		RendererSkyboxes();

		~RendererSkyboxes();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void PrepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void RenderSkybox(Skybox *object);

		void EndRendering();
	};
}
