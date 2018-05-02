#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Text.hpp"

namespace fl
{
	class FL_EXPORT RendererFonts :
		public IRenderer
	{
	private:
		Pipeline *m_pipeline;
	public:
		RendererFonts(const GraphicsStage &graphicsStage);

		~RendererFonts();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
