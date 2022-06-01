#pragma once

#include "Graphics/Subrender.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Ui/Export.hpp"

namespace acid {
class ACID_UI_EXPORT FontsSubrender : public Subrender {
public:
	explicit FontsSubrender(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	PipelineGraphics pipeline;
};
}
