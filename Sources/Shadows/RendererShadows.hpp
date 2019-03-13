#pragma once

#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Renderer/RenderPipeline.hpp"

namespace acid
{
class ACID_EXPORT RendererShadows : public RenderPipeline
{
  public:
	static const uint32_t Cascades;
	static const float BiasConstants;
	static const float BiasSlope;

	explicit RendererShadows(const Pipeline::Stage& pipelineStage);

	void Render(const CommandBuffer& commandBuffer) override;

  private:
	std::vector<Shader::Define> GetDefines();

	PipelineGraphics m_pipeline;
	UniformHandler m_uniformScene;
};
}
