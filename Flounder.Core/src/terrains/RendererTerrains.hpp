#pragma once

#include "../renderer/IRenderer.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"

namespace Flounder
{
	class RendererTerrains :
		public IRenderer
	{
	private:
		UniformBuffer *m_uniformScene;

		Shader *m_shader;
		Pipeline *m_pipeline;
	public:
		RendererTerrains();

		~RendererTerrains();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
