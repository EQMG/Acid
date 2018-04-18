#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Models/Model.hpp"
#include "Shadows.hpp"

namespace Flounder
{
	class F_EXPORT RendererShadows :
		public IRenderer
	{
	private:
		Pipeline *m_pipeline;
		UniformHandler *m_uniformScene;
	public:
		RendererShadows(const GraphicsStage &graphicsStage);

		~RendererShadows();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
