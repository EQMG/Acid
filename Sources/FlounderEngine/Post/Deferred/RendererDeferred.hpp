#pragma once

#include "../../Renderer/IRenderer.hpp"
#include "../../Renderer/Buffers/UniformBuffer.hpp"
#include "../../Renderer/Pipelines/Pipeline.hpp"
#include "../../Models/Model.hpp"

namespace Flounder
{
	class F_EXPORT RendererDeferred :
		public IRenderer
	{
	private:
		UniformBuffer *m_uniformScene;
		Pipeline *m_pipeline;
		Model *m_model;
	public:
		RendererDeferred(const GraphicsStage &graphicsStage);

		~RendererDeferred();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
