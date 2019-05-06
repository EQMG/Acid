#pragma once

#include "Renderer/Render.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT RenderGuis :
	public Render
{
public:
	explicit RenderGuis(const Pipeline::Stage &pipelineStage);

	void Record(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics m_pipeline;
	UniformHandler m_uniformScene;
};
}
