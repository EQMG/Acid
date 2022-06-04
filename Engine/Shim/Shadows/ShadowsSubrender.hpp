#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "ShimExport.hpp"

namespace acid {
class ACID_SHIM_EXPORT ShadowsSubrender : public Subrender {
public:
	explicit ShadowsSubrender(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics pipeline;
};
}
