#pragma once

#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/RenderPipeline.hpp"

namespace acid
{
class ACID_EXPORT RendererGizmos : public RenderPipeline
{
  public:
	explicit RendererGizmos(const Pipeline::Stage& pipelineStage);

	void Render(const CommandBuffer& commandBuffer) override;

  private:
	PipelineGraphics m_pipeline;
	UniformHandler m_uniformScene;
};
}
