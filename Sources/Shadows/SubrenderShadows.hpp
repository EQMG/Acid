#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class SubrenderShadows :
	public Subrender
{
public:
	explicit SubrenderShadows(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics m_pipeline;
};
}
