#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

namespace acid {
class ACID_EXPORT
SubrenderFonts
:
public
Subrender
{
public:
	explicit SubrenderFonts(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics m_pipeline;
};
}
