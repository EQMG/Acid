#pragma once

#include "../renderer/IRenderer.hpp"

namespace Flounder
{
	class RendererTest :
		public IRenderer
	{
	private:
		struct UboTest
		{
			bool memes;
			Matrix4 projection;
		};

		Shader *m_shader;
		Pipeline *m_pipeline;
		VertexBuffer *m_vertexBuffer;
		IndexBuffer *m_indexBuffer;
	public:
		RendererTest(VkRenderPass renderPass, CommandPool *commandPoolTransfer);

		~RendererTest();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
