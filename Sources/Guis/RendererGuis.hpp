#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
	class ACID_EXPORT RendererGuis :
		public IRenderer
	{
	private:
		Pipeline m_pipeline;
	public:
		RendererGuis(const GraphicsStage &graphicsStage);

		~RendererGuis();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
