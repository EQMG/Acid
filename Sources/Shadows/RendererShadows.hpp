#pragma once

#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT RendererShadows :
	public RenderPipeline
{
public:
	explicit RendererShadows(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	std::vector<Shader::Define> GetDefines();

	PipelineGraphics m_pipeline;
};
}
