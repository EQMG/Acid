#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "UiExport.hpp"

namespace acid {
class ACID_UI_EXPORT GuisSubrender : public Subrender {
public:
	explicit GuisSubrender(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics pipeline;
};
}
