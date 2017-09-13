#pragma once

#include "../renderer/IRenderer.hpp"

namespace Flounder
{
	class RendererTest :
		public IRenderer
	{
	private:
		Shader *m_shader;
		VertexBuffer *m_vertexBuffer;
		VertexBuffer *m_vertexBuffer1;
	public:
		RendererTest(CommandPool *commandPoolTransfer);

		~RendererTest();

		void CreatePipeline(std::vector<VkPipelineShaderStageCreateInfo> *shaderStages) override;

		void CreateCommands(size_t i, std::vector<VkCommandBuffer> commandBuffers, VkPipeline graphicsPipeline) override;

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
