#pragma once

#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
	class ACID_EXPORT RendererFonts2 :
		public RenderPipeline
	{
	public:
		explicit RendererFonts2(const Pipeline::Stage &pipelineStage);

		void Render(const CommandBuffer &commandBuffer) override;
	private:
		PipelineGraphics m_pipeline;
	};
}
