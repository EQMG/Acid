#pragma once

#include "../renderer/IRenderer.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"
#include "../shaders/Shader.hpp"
#include "../textures/Texture.hpp"
#include "../models/Model.hpp"
#include "../maths/Colour.hpp"

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
		Model m_model;
		Texture m_texture;

		Shader m_shader;
		Pipeline m_pipeline;
	public:
		RendererTest();

		~RendererTest();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
