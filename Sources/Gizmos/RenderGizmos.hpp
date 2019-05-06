#pragma once

#include "Renderer/Render.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT RenderGizmos :
	public Render
{
public:
	explicit RenderGizmos(const Pipeline::Stage &pipelineStage);

	void Record(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics m_pipeline;
	UniformHandler m_uniformScene;
};
}
