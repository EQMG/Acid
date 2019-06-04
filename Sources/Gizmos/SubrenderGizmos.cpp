#include "SubrenderGizmos.hpp"

#include "Models/VertexDefault.hpp"
#include "Scenes/Scenes.hpp"
#include "Gizmos.hpp"

namespace acid
{
SubrenderGizmos::SubrenderGizmos(const Pipeline::Stage &pipelineStage) :
	Subrender{pipelineStage},
	m_pipeline{pipelineStage, {"Shaders/Gizmos/Gizmo.vert", "Shaders/Gizmos/Gizmo.frag"}, {VertexDefault::GetVertexInput(0), GizmoType::Instance::GetVertexInput(1)}, {},
	PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::ReadWrite, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE}
{
}

void SubrenderGizmos::Render(const CommandBuffer &commandBuffer)
{
	auto camera = Scenes::Get()->GetCamera();
	m_uniformScene.Push("projection", camera->GetProjectionMatrix());
	m_uniformScene.Push("view", camera->GetViewMatrix());

	auto &gizmos = Gizmos::Get()->GetGizmos();

	m_pipeline.BindPipeline(commandBuffer);

	for (const auto &[type, typeGizmos] : gizmos)
	{
		type->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
	}
}
}
