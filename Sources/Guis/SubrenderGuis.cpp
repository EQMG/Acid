#include "SubrenderGuis.hpp"

#include "Models/VertexDefault.hpp"
#include "Uis/Uis.hpp"
#include "Gui.hpp"

namespace acid
{
SubrenderGuis::SubrenderGuis(const Pipeline::Stage &pipelineStage) :
	Subrender{pipelineStage},
	m_pipeline{pipelineStage, {"Shaders/Guis/Gui.vert", "Shaders/Guis/Gui.frag"}, {VertexDefault::GetVertexInput()}}
{
}

void SubrenderGuis::Render(const CommandBuffer &commandBuffer)
{
	m_pipeline.BindPipeline(commandBuffer);

	for (const auto &screenObject : Uis::Get()->GetObjects())
	{
		if (!screenObject->IsEnabled())
		{
			continue;
		}

		auto object = dynamic_cast<Gui *>(screenObject);

		if (object)
		{
			object->CmdRender(commandBuffer, m_pipeline);
		}
	}
}
}
