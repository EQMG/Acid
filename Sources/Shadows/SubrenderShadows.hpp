#pragma once

#include "Renderer/Subrender.hpp"
#include "Renderer/Buffers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"

namespace acid
{
class ACID_EXPORT SubrenderShadows :
	public Subrender
{
public:
	explicit SubrenderShadows(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	std::vector<Shader::Define> GetDefines();

	PipelineGraphics m_pipeline;
};
}
