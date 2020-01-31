#include "SubrenderGuis.hpp"

#include "Models/Vertex2d.hpp"
#include "Uis/Uis.hpp"
#include "Gui.hpp"

namespace acid {
SubrenderGuis::SubrenderGuis(const Pipeline::Stage &pipelineStage) :
	Subrender(pipelineStage),
	m_pipeline(pipelineStage, {"Shaders/Guis/Gui.vert", "Shaders/Guis/Gui.frag"}, {Vertex2d::GetVertexInput()}) {
}

void SubrenderGuis::Render(const CommandBuffer &commandBuffer) {
	m_pipeline.BindPipeline(commandBuffer);

	for (const auto &screenObject : Uis::Get()->GetObjects()) {
		if (!screenObject->IsEnabled()) {
			continue;
		}

		if (auto object = dynamic_cast<Gui *>(screenObject)) {
			object->CmdRender(commandBuffer, m_pipeline);
		}
	}
}
}
