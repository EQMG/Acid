#pragma once

#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
	class ACID_EXPORT RendererParticles :
		public RenderPipeline
	{
	public:
		explicit RendererParticles(const Pipeline::Stage &pipelineStage);

		void Render(const CommandBuffer &commandBuffer) override;
	private:
		PipelineGraphics m_pipeline;
		UniformHandler m_uniformScene;
	};
}
