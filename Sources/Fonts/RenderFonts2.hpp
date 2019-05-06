#pragma once

#include "Renderer/Render.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT RenderFonts2 :
	public Render
{
public:
	explicit RenderFonts2(const Pipeline::Stage &pipelineStage);

	void Record(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics m_pipeline;
};
}
