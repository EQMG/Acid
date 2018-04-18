#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class F_EXPORT RendererVoxels :
		public IRenderer
	{
	private:
		Pipeline *m_pipeline;
		UniformHandler *m_uniformScene;
	public:
		RendererVoxels(const GraphicsStage &graphicsStage);

		~RendererVoxels();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
