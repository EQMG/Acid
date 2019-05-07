#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

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
