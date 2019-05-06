#include "RenderGuis.hpp"

#include "Models/VertexDefault.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis/Uis.hpp"
#include "Gui.hpp"

namespace acid
{
RenderGuis::RenderGuis(const Pipeline::Stage &pipelineStage) :
	Render(pipelineStage),
	m_pipeline(pipelineStage, { "Shaders/Guis/Gui.vert", "Shaders/Guis/Gui.frag" }, { VertexDefault::GetVertexInput() })
{
}

void RenderGuis::Record(const CommandBuffer &commandBuffer)
{
	auto camera = Scenes::Get()->GetCamera();
	m_uniformScene.Push("projection", camera->GetProjectionMatrix());
	m_uniformScene.Push("view", camera->GetViewMatrix());

	m_pipeline.BindPipeline(commandBuffer);

	for (const auto &screenObject : Uis::Get()->GetObjects())
	{
		if (!screenObject->IsEnabled())
		{
			continue;
		}

		auto object = dynamic_cast<Gui *>(screenObject);

		if (object != nullptr)
		{
			object->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}
	}
}
}
