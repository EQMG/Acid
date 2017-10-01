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
		struct UniformCamera // mat4 of vec4 of float32, mat4 of vec4 of float32, mat4 of vec4 of float32
		{
			Matrix4 projection;
			Matrix4 view;
			Vector4 clip;
		};

		struct UniformObject // uint32, vec4 of float32, mat4 of vec4 of float32
		{
			bool memes;
			Colour colour;
			//Matrix4 model;
		};

		//UniformBuffer m_cameraUniformBuffer;
		UniformBuffer m_objectUniformBuffer;
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
