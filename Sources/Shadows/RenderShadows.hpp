#pragma once

#include "Renderer/Render.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT RenderShadows :
	public Render
{
public:
	explicit RenderShadows(const Pipeline::Stage &pipelineStage);

	void Record(const CommandBuffer &commandBuffer) override;

private:
	std::vector<Shader::Define> GetDefines();

	PipelineGraphics m_pipeline;
};
}
