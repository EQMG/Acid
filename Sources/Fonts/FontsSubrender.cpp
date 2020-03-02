#include "FontsSubrender.hpp"

#include "Uis/Uis.hpp"
#include "Text.hpp"

namespace acid {
FontsSubrender::FontsSubrender(const Pipeline::Stage &pipelineStage) :
	Subrender(pipelineStage),
	pipeline(pipelineStage, {"Shaders/Fonts/Font.vert", "Shaders/Fonts/Font.frag"}, {VertexText::GetVertexInput()}) {
}

void FontsSubrender::Render(const CommandBuffer &commandBuffer) {
	pipeline.BindPipeline(commandBuffer);

	for (const auto &screenObject : Uis::Get()->GetObjects()) {
		if (!screenObject->IsEnabled()) {
			continue;
		}

		if (auto object = dynamic_cast<Text *>(screenObject)) {
			object->CmdRender(commandBuffer, pipeline);
		}
	}
}
}
