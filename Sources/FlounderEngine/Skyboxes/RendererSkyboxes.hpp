#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class F_EXPORT RendererSkyboxes :
		public IRenderer
	{
	private:
		Pipeline *m_pipeline;
		UniformHandler *m_uniformScene;
	public:
		RendererSkyboxes(const GraphicsStage &graphicsStage);

		~RendererSkyboxes();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
