#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Buffers/UniformHandler.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "ShimExport.hpp"

namespace acid {
class ACID_SHIM_EXPORT ParticlesSubrender : public Subrender {
public:
	explicit ParticlesSubrender(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics pipeline;
	UniformHandler uniformScene;
};
}
