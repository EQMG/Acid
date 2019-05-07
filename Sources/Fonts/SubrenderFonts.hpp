#pragma once

#include "Renderer/Subrender.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT SubrenderFonts :
	public Subrender
{
public:
	explicit SubrenderFonts(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics m_pipeline;
	UniformHandler m_uniformScene;
};
}
