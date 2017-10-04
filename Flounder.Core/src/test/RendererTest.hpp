#pragma once

#include "../renderer/IRenderer.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"
#include "../shaders/Shader.hpp"
#include "../textures/Texture.hpp"
#include "../models/Model.hpp"

namespace Flounder
{
	class RendererTest :
		public IRenderer
	{
	private:
		struct UboScene
		{
			Matrix4 projection;
			Matrix4 view;
			Vector4 clip;
		};

		struct UboObject
		{
			Matrix4 model;
			float swaying;
			Vector2 swayOffset;
		};

		UniformBuffer m_uniformScene;

		UniformBuffer m_uniformObject;
		Model m_model;
		Texture m_diffuse;
		Texture m_swapMap;

		Shader m_shader;
		Pipeline m_pipeline;
	public:
		RendererTest();

		~RendererTest();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
