#pragma once

#include "../renderer/IRenderer.hpp"
#include "../renderer/buffers/IndexBuffer.hpp"
#include "../renderer/buffers/VertexBuffer.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../shaders/Shader.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	class RendererTest :
		public IRenderer
	{
	private:
		struct UBO
		{
			Matrix4 projection;
			Matrix4 view;
			Vector4 clip;
			Matrix4 model;
		};

		UniformBuffer m_uniformBuffer;
		Texture m_texture;

		Shader m_shader;
		Pipeline m_pipeline;
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;
	public:
		RendererTest();

		~RendererTest();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
