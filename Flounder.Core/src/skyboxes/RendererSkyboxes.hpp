#pragma once

#include "../renderer/IRenderer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"
#include "../shaders/Shader.hpp"

#include "Skyboxes.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"

namespace Flounder
{
	class RendererSkyboxes :
		public IRenderer
	{
	private:
		UniformBuffer *m_uniformScene;

		Shader *m_shader;
		Pipeline *m_pipeline;
	public:
		RendererSkyboxes();

		~RendererSkyboxes();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
