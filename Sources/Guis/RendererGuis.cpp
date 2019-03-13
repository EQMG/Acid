#include "RendererGuis.hpp"

#include "Gui.hpp"
#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
RendererGuis::RendererGuis(const Pipeline::Stage& pipelineStage) : RenderPipeline(pipelineStage), m_pipeline(pipelineStage, {"Shaders/Guis/Gui.vert", "Shaders/Guis/Gui.frag"}, {VertexModel::GetVertexInput()}) {}

void RendererGuis::Render(const CommandBuffer& commandBuffer)
{
	auto camera = Scenes::Get()->GetCamera();
	m_uniformScene.Push("projection", camera->GetProjectionMatrix());
	m_uniformScene.Push("view", camera->GetViewMatrix());

	m_pipeline.BindPipeline(commandBuffer);

	for(const auto& screenObject : Uis::Get()->GetObjects())
		{
			if(!screenObject->IsEnabled())
				{
					continue;
				}

			auto object = dynamic_cast<Gui*>(screenObject);

			if(object != nullptr)
				{
					object->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
				}
		}
}
}
