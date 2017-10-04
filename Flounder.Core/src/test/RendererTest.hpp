#pragma once

#include "../renderer/IRenderer.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"
#include "../shaders/Shader.hpp"

#include "TestEntity.hpp"

namespace Flounder
{
	class RendererTest :
		public IRenderer
	{
	private:
		UniformBuffer m_uniformScene;

		TestEntity m_testEntity;

		Shader m_shader;
		Pipeline m_pipeline;
	public:
		RendererTest();

		~RendererTest();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
