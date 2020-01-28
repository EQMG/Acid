#include "SubrenderFonts.hpp"

#include "Models/Vertex2.hpp"
#include "Uis/Uis.hpp"
#include "Text.hpp"

namespace acid {
SubrenderFonts::SubrenderFonts(const Pipeline::Stage &pipelineStage) :
	Subrender(pipelineStage),
	m_pipeline(pipelineStage, {"Shaders/Fonts/Font.vert", "Shaders/Fonts/Font.frag"}, {Vertex2::GetVertexInput()}) {
}

void SubrenderFonts::Render(const CommandBuffer &commandBuffer) {
	m_pipeline.BindPipeline(commandBuffer);

	for (const auto &screenObject : Uis::Get()->GetObjects()) {
		if (!screenObject->IsEnabled()) {
			continue;
		}

		if (auto object = dynamic_cast<Text *>(screenObject)) {
			object->CmdRender(commandBuffer, m_pipeline);
		}
	}
}
}
