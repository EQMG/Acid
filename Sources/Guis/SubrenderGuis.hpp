#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT SubrenderGuis :
	public Subrender
{
public:
	explicit SubrenderGuis(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics m_pipeline;
	UniformHandler m_uniformScene;
};
}
