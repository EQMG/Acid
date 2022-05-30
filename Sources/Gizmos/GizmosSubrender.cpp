#include "GizmosSubrender.hpp"

#include "Models/Vertex3d.hpp"
#include "Scenes/Scenes.hpp"
#include "Gizmos.hpp"

namespace acid {
GizmosSubrender::GizmosSubrender(const Pipeline::Stage &pipelineStage) :
	Subrender(pipelineStage),
	pipeline(pipelineStage, {"Shaders/Gizmos/Gizmo.vert", "Shaders/Gizmos/Gizmo.frag"}, {Vertex3d::GetVertexInput(0), GizmoType::Instance::GetVertexInput(1)}, {},
		PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::ReadWrite, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE) {
}

void GizmosSubrender::Render(const CommandBuffer &commandBuffer) {
	if (!Scenes::Get()->GetScene()->GetSystem<Gizmos>())
		return;

	auto camera = Scenes::Get()->GetScene()->GetCamera();
	uniformScene.Push("projection", camera->GetProjectionMatrix());
	uniformScene.Push("view", camera->GetViewMatrix());

	auto &gizmos = Scenes::Get()->GetScene()->GetSystem<Gizmos>()->GetGizmos();

	pipeline.BindPipeline(commandBuffer);

	for (const auto &[type, typeGizmos] : gizmos)
		type->CmdRender(commandBuffer, pipeline, uniformScene);
}
}
