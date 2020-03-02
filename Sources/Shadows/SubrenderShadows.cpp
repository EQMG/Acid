#include "SubrenderShadows.hpp"

#include "Models/Vertex3d.hpp"
#include "Scenes/Scenes.hpp"
#include "ShadowRender.hpp"
#include "Shadows.hpp"

namespace acid {
SubrenderShadows::SubrenderShadows(const Pipeline::Stage &pipelineStage) :
	Subrender(pipelineStage),
	pipeline(pipelineStage, {"Shaders/Shadows/Shadow.vert", "Shaders/Shadows/Shadow.frag"}, {Vertex3d::GetVertexInput()}, {},
		PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT) {
}

void SubrenderShadows::Render(const CommandBuffer &commandBuffer) {
	auto camera = Scenes::Get()->GetCamera();

	pipeline.BindPipeline(commandBuffer);

	auto sceneShadowRenders = Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>();

	for (const auto &shadowRender : sceneShadowRenders) {
		shadowRender->CmdRender(commandBuffer, pipeline);
	}
}
}
