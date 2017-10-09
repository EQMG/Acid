#pragma once

#include "../renderer/IRenderer.hpp"
#include "Waters.hpp"

namespace Flounder
{
	class RendererWaters :
		public IRenderer
	{
	private:
		UniformBuffer *m_uniformScene;

		Shader *m_shader;
		Pipeline *m_pipeline;
	public:
		RendererWaters();

		~RendererWaters();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
