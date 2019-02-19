#include "RendererGuis.hpp"

#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis/Uis.hpp"
#include "Gui.hpp"

namespace acid
{
	RendererGuis::RendererGuis(const Pipeline::Stage &pipelineStage) :
		RenderPipeline(pipelineStage),
		m_pipeline(PipelineGraphics(pipelineStage, {"Shaders/Guis/Gui.vert", "Shaders/Guis/Gui.frag"}, {VertexModel::GetVertexInput()},
		                            PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::ReadWrite, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, false, {})),
		m_uniformScene(UniformHandler())
	{
	}

	void RendererGuis::Render(const CommandBuffer &commandBuffer)
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

			Gui *object = dynamic_cast<Gui *>(screenObject);

			if (object != nullptr)
			{
				object->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
			}
		}
	}
}
