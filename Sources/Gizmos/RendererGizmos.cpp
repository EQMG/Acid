#include "RendererGizmos.hpp"

#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"
#include "Gizmos.hpp"

namespace acid
{
	RendererGizmos::RendererGizmos(const GraphicsStage &graphicsStage) :
		RenderPipeline(graphicsStage),
		m_pipeline(PipelineGraphics(graphicsStage, {"Shaders/Gizmos/Gizmo.vert", "Shaders/Gizmos/Gizmo.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, false, {})),
		m_uniformScene(UniformHandler())
	{
	}

	void RendererGizmos::Render(const CommandBuffer &commandBuffer)
	{
		auto camera = Scenes::Get()->GetCamera();
		m_uniformScene.Push("projection", camera->GetProjectionMatrix());
		m_uniformScene.Push("view", camera->GetViewMatrix());

		auto &gizmos = Gizmos::Get()->GetGizmos();

		m_pipeline.BindPipeline(commandBuffer);

		for (const auto &[type, gizmos] : gizmos)
		{
			type->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}
	}
}
