#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class F_EXPORT RendererWaters :
		public IRenderer
	{
	private:
		UniformBuffer *m_uniformScene;
		Pipeline *m_pipeline;
	public:
		RendererWaters(const GraphicsStage &graphicsStage);

		~RendererWaters();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
