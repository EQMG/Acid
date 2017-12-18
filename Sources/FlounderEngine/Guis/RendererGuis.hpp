#pragma once

#include "../Renderer/IRenderer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class RendererGuis :
		public IRenderer
	{
	private:
		Pipeline *m_pipeline;
	public:
		RendererGuis(const int &subpass);

		~RendererGuis();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
