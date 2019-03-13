#pragma once

#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/RenderPipeline.hpp"

namespace acid
{
class ACID_EXPORT RendererGuis : public RenderPipeline
{
  public:
	explicit RendererGuis(const Pipeline::Stage& pipelineStage);

	void Render(const CommandBuffer& commandBuffer) override;

  private:
	PipelineGraphics m_pipeline;
	UniformHandler m_uniformScene;
};
}
